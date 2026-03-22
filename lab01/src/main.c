#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "sieve.h"
#include "timing.h"

int main() {
    int N = 10000000;
    int trials = 1000;

    const char *output = "output";
    struct stat st = {0};
    if (stat(output, &st) == -1)
        mkdir(output, 0755);

    const char *csv_path = "output/results.csv";

    FILE *f = fopen(csv_path, "w");

    fprintf(f, "Trial,RDTSC_ms,CLOCK_MONOTONIC_ms\n");

    for (int t = 0; t < trials; t++) {
        unsigned long long RDTSC_ms = measure_rdtsc(N);
        long long CLOCK_MONOTONIC_ms = measure_clock(N);
        fprintf(f, "%d,%llu,%lld\n", t + 1, RDTSC_ms, CLOCK_MONOTONIC_ms);
    }

    fclose(f);
    printf("Results saved to %s\n", csv_path);

    return 0;
}