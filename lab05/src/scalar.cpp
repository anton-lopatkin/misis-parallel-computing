#include "scalar.hpp"
#include <immintrin.h>

static float scalar1(const int8_t* a, int N) {
    __m128d acc = _mm_setzero_pd();
    for (int i = 0; i < N; i++) {
        __m128d v = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i]);
        acc = _mm_add_sd(acc, _mm_mul_sd(v, v));
    }
    __m128 f = _mm_cvtsd_ss(_mm_setzero_ps(), acc);
    return _mm_cvtss_f32(_mm_sqrt_ss(f));
}

static float scalar2(const int8_t* a, int N) {
    __m128d acc0 = _mm_setzero_pd(), acc1 = _mm_setzero_pd();
    for (int i = 0; i < N; i += 2) {
        __m128d v0 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i]);
        __m128d v1 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 1]);
        acc0 = _mm_add_sd(acc0, _mm_mul_sd(v0, v0));
        acc1 = _mm_add_sd(acc1, _mm_mul_sd(v1, v1));
    }
    __m128d sum = _mm_add_sd(acc0, acc1);
    __m128 f = _mm_cvtsd_ss(_mm_setzero_ps(), sum);
    return _mm_cvtss_f32(_mm_sqrt_ss(f));
}

static float scalar4(const int8_t* a, int N) {
    __m128d acc0 = _mm_setzero_pd(), acc1 = _mm_setzero_pd();
    __m128d acc2 = _mm_setzero_pd(), acc3 = _mm_setzero_pd();
    for (int i = 0; i < N; i += 4) {
        __m128d v0 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i]);
        __m128d v1 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 1]);
        __m128d v2 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 2]);
        __m128d v3 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 3]);
        acc0 = _mm_add_sd(acc0, _mm_mul_sd(v0, v0));
        acc1 = _mm_add_sd(acc1, _mm_mul_sd(v1, v1));
        acc2 = _mm_add_sd(acc2, _mm_mul_sd(v2, v2));
        acc3 = _mm_add_sd(acc3, _mm_mul_sd(v3, v3));
    }
    __m128d sum = _mm_add_sd(_mm_add_sd(acc0, acc1), _mm_add_sd(acc2, acc3));
    __m128 f = _mm_cvtsd_ss(_mm_setzero_ps(), sum);
    return _mm_cvtss_f32(_mm_sqrt_ss(f));
}

static float scalar8(const int8_t* a, int N) {
    __m128d acc0 = _mm_setzero_pd(), acc1 = _mm_setzero_pd();
    __m128d acc2 = _mm_setzero_pd(), acc3 = _mm_setzero_pd();
    __m128d acc4 = _mm_setzero_pd(), acc5 = _mm_setzero_pd();
    __m128d acc6 = _mm_setzero_pd(), acc7 = _mm_setzero_pd();
    for (int i = 0; i < N; i += 8) {
        __m128d v0 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i]);
        __m128d v1 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 1]);
        __m128d v2 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 2]);
        __m128d v3 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 3]);
        __m128d v4 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 4]);
        __m128d v5 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 5]);
        __m128d v6 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 6]);
        __m128d v7 = _mm_cvtsi32_sd(_mm_setzero_pd(), a[i + 7]);
        acc0 = _mm_add_sd(acc0, _mm_mul_sd(v0, v0));
        acc1 = _mm_add_sd(acc1, _mm_mul_sd(v1, v1));
        acc2 = _mm_add_sd(acc2, _mm_mul_sd(v2, v2));
        acc3 = _mm_add_sd(acc3, _mm_mul_sd(v3, v3));
        acc4 = _mm_add_sd(acc4, _mm_mul_sd(v4, v4));
        acc5 = _mm_add_sd(acc5, _mm_mul_sd(v5, v5));
        acc6 = _mm_add_sd(acc6, _mm_mul_sd(v6, v6));
        acc7 = _mm_add_sd(acc7, _mm_mul_sd(v7, v7));
    }
    __m128d sum = _mm_add_sd(
        _mm_add_sd(_mm_add_sd(acc0, acc1), _mm_add_sd(acc2, acc3)),
        _mm_add_sd(_mm_add_sd(acc4, acc5), _mm_add_sd(acc6, acc7))
    );
    __m128 f = _mm_cvtsd_ss(_mm_setzero_ps(), sum);
    return _mm_cvtss_f32(_mm_sqrt_ss(f));
}

float normScalar(const int8_t* a, int N, int M) {
    switch (M) {
        case 2:  return scalar2(a, N);
        case 4:  return scalar4(a, N);
        case 8:  return scalar8(a, N);
        default: return scalar1(a, N);
    }
}
