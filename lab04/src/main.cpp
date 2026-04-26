#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <string>
#include <cstdint>

#include "utils.hpp"
#include "seq.hpp"
#include "rand.hpp"
#include "randidx.hpp"

template<typename Func>
double nsPerElem(Func func, int N, int runs = 15) {
    double best = 1e18;
    for (int r = 0; r < runs; r++) {
        auto t0 = std::chrono::high_resolution_clock::now();
        func();
        auto t1 = std::chrono::high_resolution_clock::now();
        best = std::min(best, std::chrono::duration<double>(t1 - t0).count());
    }
    return best * 1e9 / N;
}

void row(std::ofstream& f, const std::string& range, const std::string& method, int sizeKB, double ns) {
    f << range << "," << method << "," << sizeKB << "," << ns << "\n";
    std::cout << method << " " << sizeKB << " KB - " << ns << " ns/elem\n";
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
    f << "range,method,size_kb,ns\n";

    auto measure = [&](const std::string& range, int kb) {
        const int N = kb * 1024 / static_cast<int>(sizeof(int32_t));
        buildIdx(idx, N);
        row(f, range, "seq", kb, nsPerElem([&]{ sumSeq(a, N); }, N));
        row(f, range, "rand", kb, nsPerElem([&]{ sumRand(a, N); }, N));
        row(f, range, "randidx", kb, nsPerElem([&]{ sumRandIdx(a, idx, N); }, N));
    };

    for (int kb = 1; kb <= 2048; kb++)
        measure("fine", kb);

    for (int kb = 512; kb <= 32 * 1024; kb += 512)
        measure("medium", kb);

    for (int kb = 5 * 1024; kb <= 150 * 1024; kb += 5 * 1024)
        measure("coarse", kb);

    deleteArray(a);
    deleteArray(idx);

    std::cout << "\nResults saved to " << filename << "\n";
    return 0;
}
