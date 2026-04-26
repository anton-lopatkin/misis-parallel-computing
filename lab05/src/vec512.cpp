#include "vec512.hpp"
#include <immintrin.h>

static inline int64_t hsumEpi32_512(__m512i v) {
    __m256i lo = _mm512_castsi512_si256(v);
    __m256i hi = _mm512_extracti64x4_epi64(v, 1);
    __m512i lo64 = _mm512_cvtepi32_epi64(lo);
    __m512i hi64 = _mm512_cvtepi32_epi64(hi);
    return _mm512_reduce_add_epi64(_mm512_add_epi64(lo64, hi64));
}

static inline float sqrtOfSum512(int64_t sum) {
    __m512 v = _mm512_set1_ps(static_cast<float>(sum));
    return _mm_cvtss_f32(_mm512_castps512_ps128(_mm512_sqrt_ps(v)));
}

static inline void accumulate512(__m512i& acc, const int8_t* p) {
    __m512i v    = _mm512_loadu_si512(p);
    __m256i lo   = _mm512_castsi512_si256(v);
    __m256i hi   = _mm512_extracti64x4_epi64(v, 1);
    __m512i lo16 = _mm512_cvtepi8_epi16(lo);
    __m512i hi16 = _mm512_cvtepi8_epi16(hi);
    acc = _mm512_add_epi32(acc, _mm512_madd_epi16(lo16, lo16));
    acc = _mm512_add_epi32(acc, _mm512_madd_epi16(hi16, hi16));
}

static float vec512_1(const int8_t* a, int N) {
    __m512i acc = _mm512_setzero_si512();
    for (int i = 0; i < N; i += 64)
        accumulate512(acc, a + i);
    return sqrtOfSum512(hsumEpi32_512(acc));
}

static float vec512_2(const int8_t* a, int N) {
    __m512i acc0 = _mm512_setzero_si512(), acc1 = _mm512_setzero_si512();
    for (int i = 0; i < N; i += 128) {
        accumulate512(acc0, a + i);
        accumulate512(acc1, a + i + 64);
    }
    return sqrtOfSum512(hsumEpi32_512(_mm512_add_epi32(acc0, acc1)));
}

static float vec512_4(const int8_t* a, int N) {
    __m512i acc0 = _mm512_setzero_si512(), acc1 = _mm512_setzero_si512();
    __m512i acc2 = _mm512_setzero_si512(), acc3 = _mm512_setzero_si512();
    for (int i = 0; i < N; i += 256) {
        accumulate512(acc0, a + i);
        accumulate512(acc1, a + i + 64);
        accumulate512(acc2, a + i + 128);
        accumulate512(acc3, a + i + 192);
    }
    __m512i acc = _mm512_add_epi32(_mm512_add_epi32(acc0, acc1), _mm512_add_epi32(acc2, acc3));
    return sqrtOfSum512(hsumEpi32_512(acc));
}

static float vec512_8(const int8_t* a, int N) {
    __m512i acc0 = _mm512_setzero_si512(), acc1 = _mm512_setzero_si512();
    __m512i acc2 = _mm512_setzero_si512(), acc3 = _mm512_setzero_si512();
    __m512i acc4 = _mm512_setzero_si512(), acc5 = _mm512_setzero_si512();
    __m512i acc6 = _mm512_setzero_si512(), acc7 = _mm512_setzero_si512();
    for (int i = 0; i < N; i += 512) {
        accumulate512(acc0, a + i);
        accumulate512(acc1, a + i + 64);
        accumulate512(acc2, a + i + 128);
        accumulate512(acc3, a + i + 192);
        accumulate512(acc4, a + i + 256);
        accumulate512(acc5, a + i + 320);
        accumulate512(acc6, a + i + 384);
        accumulate512(acc7, a + i + 448);
    }
    __m512i acc = _mm512_add_epi32(
        _mm512_add_epi32(_mm512_add_epi32(acc0, acc1), _mm512_add_epi32(acc2, acc3)),
        _mm512_add_epi32(_mm512_add_epi32(acc4, acc5), _mm512_add_epi32(acc6, acc7))
    );
    return sqrtOfSum512(hsumEpi32_512(acc));
}

float normVec512(const int8_t* a, int N, int M) {
    switch (M) {
        case 2:  return vec512_2(a, N);
        case 4:  return vec512_4(a, N);
        case 8:  return vec512_8(a, N);
        default: return vec512_1(a, N);
    }
}
