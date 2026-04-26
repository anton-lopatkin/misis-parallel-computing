#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <string>

#include "plain.hpp"
#include "scalar.hpp"
#include "vec.hpp"
#include "vec512.hpp"

int8_t* createRandomArray(int N) {
    int8_t* a = static_cast<int8_t*>(aligned_alloc(64, N));
    for (int i = 0; i < N; i++)
        a[i] = static_cast<int8_t>((rand() & 0xFF) - 128);
    return a;
}

bool compare(float a, float b, float eps = 2.0f) {
    return fabsf(a - b) <= eps;
}

template<typename Func>
double measureTime(Func func, int runs = 100) {
    double best = 1e18;
    for (int r = 0; r < runs; r++) {
        auto t0 = std::chrono::high_resolution_clock::now();
        func();
        auto t1 = std::chrono::high_resolution_clock::now();
        best = std::min(best, std::chrono::duration<double>(t1 - t0).count());
    }
    return best;
}

void row(const std::string& method, int M, double t, float result) {
    double ms = t * 1e3;
    std::cout << method << " M=" << M << " - " << ms << " ms, result=" << result << "\n";
}

int main(int argc, char* argv[]) {
    const int N = 1 << 20;
    int8_t* a = createRandomArray(N);

    float ref = normPlain(a, N);
    for (int M : {1, 2, 4, 8}) {
        float r = normScalar(a, N, M);
        std::cout << "Correctness scalar M=" << M << ": " << (compare(ref, r) ? "OK" : "FAIL") << "\n";
    }
    for (int M : {1, 2, 4, 8}) {
        float r = normVec(a, N, M);
        std::cout << "Correctness vec    M=" << M << ": " << (compare(ref, r) ? "OK" : "FAIL") << "\n";
    }
    for (int M : {1, 2, 4, 8}) {
        float r = normVec512(a, N, M);
        std::cout << "Correctness vec512 M=" << M << ": " << (compare(ref, r) ? "OK" : "FAIL") << "\n";
    }

    float res;
    double t;

    t = measureTime([&]{ res = normPlain(a, N); });
    row("plain", 1, t, res);

    for (int M : {1, 2, 4, 8}) {
        t = measureTime([&]{ res = normScalar(a, N, M); });
        row("scalar", M, t, res);
    }

    for (int M : {1, 2, 4, 8}) {
        t = measureTime([&]{ res = normVec(a, N, M); });
        row("vec", M, t, res);
    }

    for (int M : {1, 2, 4, 8}) {
        t = measureTime([&]{ res = normVec512(a, N, M); });
        row("vec512", M, t, res);
    }

    free(a);

    return 0;
}
