#include <x86intrin.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "sieve.h"
#include "timing.h"

unsigned long long measure_rdtsc(int N) {
    unsigned long long start = __rdtsc();

    bool* prime = malloc((N + 1) * sizeof(bool));
    sieve(N, prime);
    free(prime);

    unsigned long long end = __rdtsc();

    unsigned long long t1 = __rdtsc();
    sleep(1);
    unsigned long long t2 = __rdtsc();
    double cur_freq_hz = (double)(t2 - t1);

    return (end - start) / cur_freq_hz * 1000;
}

long long measure_clock(int N) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    bool* prime = malloc((N + 1) * sizeof(bool));
    sieve(N, prime);
    free(prime);

    clock_gettime(CLOCK_MONOTONIC, &end);

    long long elapsed_ms = (end.tv_sec - start.tv_sec) * 1000
                         + (end.tv_nsec - start.tv_nsec) / 1000000;
    return elapsed_ms;
}