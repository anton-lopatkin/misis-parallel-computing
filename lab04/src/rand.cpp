#include "rand.hpp"

int64_t sumRand(const int32_t* a, int N) {
    volatile int64_t s = 0;
    uint32_t seed = 42424242u;
    const uint32_t n = static_cast<uint32_t>(N);
    for (int i = 0; i < N; i++) {
        seed = seed * 1774525u + 1009801723u;
        s += a[seed % n];
    }
    return s;
}
