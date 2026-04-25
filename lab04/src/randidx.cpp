#include "randidx.hpp"

int64_t sumRandIdx(const int32_t* a, const int32_t* idx, int N) {
    volatile int64_t s = 0;
    for (int i = 0; i < N; i++) 
        s += a[idx[i]];
    return s;
}
