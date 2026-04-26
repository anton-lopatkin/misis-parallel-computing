#include "plain.hpp"
#include <cmath>

float normPlain(const int8_t* a, int N) {
    int64_t sum = 0;
    for (int i = 0; i < N; i++) {
        int32_t v = a[i];
        sum += v * v;
    }
    return sqrtf(static_cast<float>(sum));
}
