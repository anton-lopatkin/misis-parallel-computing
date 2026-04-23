#include "timing.hpp"
#include <cstdint>
#include <ctime>
#include <x86intrin.h>

double tsc_freq_hz = 1.0;

void tsc_init() {
    struct timespec t1, t2;
    uint64_t c1 = __rdtsc();
    clock_gettime(CLOCK_MONOTONIC, &t1);

    struct timespec req = {0, 200000000};
    nanosleep(&req, nullptr);

    uint64_t c2 = __rdtsc();
    clock_gettime(CLOCK_MONOTONIC, &t2);

    double elapsed = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) * 1e-9;
    tsc_freq_hz = (c2 - c1) / elapsed;
}

double measure_rdtsc(const std::function<void()>& task) {
    uint64_t start = __rdtsc();
    task();
    uint64_t end = __rdtsc();
    return (end - start) / tsc_freq_hz * 1000.0;
}

double measure_monotonic(const std::function<void()>& task) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    task();
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) * 1e-6;
}
