#ifndef TIMING_HPP
#define TIMING_HPP

#include <functional>

void tsc_init();

double measure_rdtsc(const std::function<void()>& task);
double measure_monotonic(const std::function<void()>& task);

#endif