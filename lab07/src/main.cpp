#include <iostream>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <string>

#include "naive.hpp"
#include "avx512.hpp"

uint8_t* createRandomImage(int N) {
    uint8_t* img = static_cast<uint8_t*>(aligned_alloc(64, N));
    for (int i = 0; i < N; i++)
        img[i] = static_cast<uint8_t>(rand() & 0xFF);
    return img;
}

uint8_t* createOutput(int N) {
    uint8_t* out = static_cast<uint8_t*>(aligned_alloc(64, N));
    memset(out, 0, N);
    return out;
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

int main() {
    const int W = 1024;
    const int H = 1024;
    const int N = W * H;
    uint8_t* img = createRandomImage(N);
    uint8_t* naiveOut = createOutput(N);
    uint8_t* simdOut = createOutput(N);

    sobelNaive(img, naiveOut, W, H);
    sobelAvx512(img, simdOut, W, H);

    bool ok = true;
    for (int i = 0; i < N; i++)
        ok = ok && (naiveOut[i] == simdOut[i]);
    std::cout << "Correctness: " << (ok ? "OK" : "FAIL") << "\n";

    double t;

    t = measureTime([&]{ sobelNaive(img, naiveOut, W, H); });
    std::cout << "naive" << " - " << t * 1e3 << " ms\n";

    t = measureTime([&]{ sobelAvx512(img, simdOut, W, H); });
    std::cout << "avx512" << " - " << t  * 1e3 << " ms\n";

    free(img);
    free(naiveOut);
    free(simdOut);

    return 0;
}
