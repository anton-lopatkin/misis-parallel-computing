#include <fstream>
#include <vector>
#include <filesystem>
#include <cstdint>
#include "sieve.hpp"
#include "timing.hpp"

static volatile std::uint64_t sink = 0;

int main() {
    const int N = 20000000;
    const int K = 1000;

    std::vector<bool> prime;
    auto task = [&]() { sink = sieve(N, prime); };

    task();

    tsc_init();

    std::vector<double> rdtsc(K), mono(K);

    for (int i = 0; i < K; i++) {
        rdtsc[i] = measure_rdtsc(task);
        mono[i]  = measure_monotonic(task);
    }

    std::filesystem::create_directories("output");
    std::ofstream f("output/results.csv");
    f << std::fixed;
    f << "Trial,RDTSC_ms,Monotonic_ms\n";
    for (int i = 0; i < K; i++)
        f << i + 1 << "," << rdtsc[i] << "," << mono[i] << "\n";

    return 0;
}
