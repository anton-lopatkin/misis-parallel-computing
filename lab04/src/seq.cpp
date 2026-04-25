#include "seq.hpp"

int64_t sumSeq(const int32_t* a, int N) {
    volatile int64_t s = 0;
    for (int i = 0; i < N; i++) 
        s += a[i];
    return s;
}
