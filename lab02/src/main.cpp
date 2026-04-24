#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <string>

#include "utils.hpp"
#include "classic.hpp"
#include "transpose.hpp"
#include "buffered.hpp"
#include "blocked.hpp"

template<typename Func>
double measureTime(Func func, int runs = 10) {
    double best = 1e18;
    for (int r = 0; r < runs; r++) {
        auto t0 = std::chrono::high_resolution_clock::now();
        func();
        auto t1 = std::chrono::high_resolution_clock::now();
        best = std::min(best, std::chrono::duration<double>(t1 - t0).count());
    }
    return best;
}

double gflops(int N, double t) {
    return 2.0 * N * N * N / (t * 1e9);
}

void row(std::ofstream& f, const std::string& method, int N, int S, int M, double t) {
    f << method << "," << N << "," << S << "," << M << "," << gflops(N, t) << "\n";
    std::cout << method << " N=" << N << " S=" << S << " M=" << M << " - " << gflops(N, t) << " GFLOPS\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <output.csv>\n";
        return 1;
    }

    std::string filename = argv[1];

    {
        const int N = 256;
        float** A  = createRandomMatrix(N);
        float** B  = createRandomMatrix(N);
        float** C0 = createZeroMatrix(N); multiplyClassic(A, B, C0, N);
        float** C1 = createZeroMatrix(N); multiplyTranspose(A, B, C1, N);
        float** C2 = createZeroMatrix(N); multiplyBuffered(A, B, C2, N, 1);
        float** C3 = createZeroMatrix(N); multiplyBlocked(A, B, C3, N, 16, 1);

        std::cout << "Correctness transpose: " << (compareMatrices(C0, C1, N) ? "OK" : "FAIL") << "\n";
        std::cout << "Correctness buffered: " << (compareMatrices(C0, C2, N) ? "OK" : "FAIL") << "\n";
        std::cout << "Correctness blocked: " << (compareMatrices(C0, C3, N) ? "OK" : "FAIL") << "\n";

        deleteMatrix(A,N);  deleteMatrix(B,N);
        deleteMatrix(C0,N); deleteMatrix(C1,N); deleteMatrix(C2,N); deleteMatrix(C3,N);
    }

    std::ofstream f(filename);
    f << "method,N,S,M,gflops\n";

    {
        const int N = 512;
        float** A  = createRandomMatrix(N);
        float** B  = createRandomMatrix(N);
        float** BT = createMatrix(N); transposeMatrix(B, BT, N);
        float** C  = createZeroMatrix(N);

        row(f, "classic", N, 0, 0, measureTime([&]{ multiplyClassic(A, B, C, N); }));
        row(f, "transpose", N, 0, 0, measureTime([&]{ multiplyTranspose(A, B, C, N); }));
        row(f, "transposed_b", N, 0, 0, measureTime([&]{ multiplyTransposedB(A, BT, C, N); }));

        deleteMatrix(A,N); deleteMatrix(B,N); deleteMatrix(BT,N); deleteMatrix(C,N);
    }

    int bestBufM = 1;
    {
        const int N = 512;
        float** A = createRandomMatrix(N);
        float** B = createRandomMatrix(N);
        float** C = createZeroMatrix(N);
        double bestG = 0;

        for (int M : {1, 2, 4, 8, 16, 32}) {
            double t = measureTime([&]{ multiplyBuffered(A, B, C, N, M); });
            row(f, "buffered", N, 0, M, t);
            if (gflops(N, t) > bestG) { bestG = gflops(N, t); bestBufM = M; }
        }

        deleteMatrix(A,N); deleteMatrix(B,N); deleteMatrix(C,N);
    }

    int bestBlkS = 64, bestBlkM = 1;
    {
        const int N = 512;
        float** A = createRandomMatrix(N);
        float** B = createRandomMatrix(N);
        float** C = createZeroMatrix(N);
        double bestG = 0;

        for (int S : {1, 2, 4, 8, 16, 32, 64, 128, 256, 512}) {
            for (int M : {1, 2, 4, 8, 16, 32}) {
                if (M > S) continue;
                double t = measureTime([&]{ fillZero(C, N); multiplyBlocked(A, B, C, N, S, M); });
                row(f, "blocked", N, S, M, t);
                if (gflops(N, t) > bestG) { bestG = gflops(N, t); bestBlkS = S; bestBlkM = M; }
            }
        }

        deleteMatrix(A,N); deleteMatrix(B,N); deleteMatrix(C,N);
    }

    for (int N : {4, 8, 16, 32, 64, 128, 256, 512, 1024}) {
        int M  = std::min(bestBufM, N);
        int S  = std::min(bestBlkS, N);
        int Ms = std::min(bestBlkM, S);

        float** A  = createRandomMatrix(N);
        float** B  = createRandomMatrix(N);
        float** C  = createZeroMatrix(N);

        row(f, "classic", N, 0, 0, measureTime([&]{ multiplyClassic(A, B, C, N); }));
        row(f, "transpose", N, 0, 0, measureTime([&]{ multiplyTranspose(A, B, C, N); }));
        row(f, "buffered", N, 0, M, measureTime([&]{ multiplyBuffered(A, B, C, N, M); }));
        row(f, "blocked", N, S, Ms, measureTime([&]{ fillZero(C, N); multiplyBlocked(A, B, C, N, S, Ms); }));

        deleteMatrix(A,N); deleteMatrix(B,N); deleteMatrix(C,N);
    }

    std::cout << "\nResults saved to " << filename << "\n";
    return 0;
}
