#ifndef TIMING_H
#define TIMING_H

#include <stdbool.h>

unsigned long long measure_rdtsc(int N);

long long measure_clock(int N);

#endif