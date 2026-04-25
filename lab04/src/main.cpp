#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cstdint>
#include <x86intrin.h>

#include "utils.hpp"
#include "seq.hpp"
#include "rand.hpp"
#include "randidx.hpp"

template<typename Func>
double cyclesPerElem(Func func, int N, int runs = 3) {
    uint64_t best = ~0ULL;
    for (int r = 0; r < runs; r++) {
        uint64_t t0 = static_cast<uint64_t>(__rdtsc());
        func();
        uint64_t elapsed = static_cast<uint64_t>(__rdtsc()) - t0;
        if (elapsed < best) best = elapsed;
    }
    return static_cast<double>(best) / N;
}

void row(std::ofstream& f, const std::string& method, int sizeKB, double cycles) {
    f << method << "," << sizeKB << "," << cycles << "\n";
    std::cout << method << " " << sizeKB << " KB - " << cycles << " cycles/elem\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <output.csv>\n";
        return 1;
    }

    std::string filename = argv[1];

    static const size_t MAX_ELEMS = 150ULL * 1024 * 1024 / sizeof(int32_t);

    int32_t* a = createArray(MAX_ELEMS);
    int32_t* idx = createArray(MAX_ELEMS);
    fillRandom(a, MAX_ELEMS);

    std::ofstream f(filename);
    f << "method,size_kb,cycles\n";

    auto measure = [&](int kb) {
        const int N = kb * 1024 / static_cast<int>(sizeof(int32_t));
        buildIdx(idx, N);
        row(f, "seq", kb, cyclesPerElem([&]{ sumSeq(a, N); }, N));
        row(f, "rand", kb, cyclesPerElem([&]{ sumRand(a, N); }, N));
        row(f, "randidx", kb, cyclesPerElem([&]{ sumRandIdx(a, idx, N); }, N));
    };

    for (int kb = 1; kb <= 2048; kb++)
        measure(kb);

    for (int kb = 2048 + 512; kb <= 32 * 1024; kb += 512)
        measure(kb);

    for (int kb = (32 + 5) * 1024; kb <= 150 * 1024; kb += 5 * 1024)
        measure(kb);

    deleteArray(a);
    deleteArray(idx);

    std::cout << "\nResults saved to " << filename << "\n";
    return 0;
}
