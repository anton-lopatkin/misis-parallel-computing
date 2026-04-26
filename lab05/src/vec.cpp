#include "vec.hpp"
#include <immintrin.h>

static inline int64_t hsumEpi32(__m256i v) {
    __m128i lo128 = _mm256_castsi256_si128(v);
    __m128i hi128 = _mm256_extracti128_si256(v, 1);
    __m128i lo64a = _mm_cvtepi32_epi64(lo128);
    __m128i lo64b = _mm_cvtepi32_epi64(_mm_shuffle_epi32(lo128, _MM_SHUFFLE(1, 0, 3, 2)));
    __m128i hi64a = _mm_cvtepi32_epi64(hi128);
    __m128i hi64b = _mm_cvtepi32_epi64(_mm_shuffle_epi32(hi128, _MM_SHUFFLE(1, 0, 3, 2)));
    __m128i sum   = _mm_add_epi64(_mm_add_epi64(lo64a, lo64b), _mm_add_epi64(hi64a, hi64b));
    int64_t arr[2];
    _mm_storeu_si128(reinterpret_cast<__m128i*>(arr), sum);
    return arr[0] + arr[1];
}

static inline float sqrtOfSum(int64_t sum) {
    __m256 v = _mm256_set1_ps(static_cast<float>(sum));
    __m256 s = _mm256_sqrt_ps(v);
    return _mm_cvtss_f32(_mm256_castps256_ps128(s));
}

static inline void accumulate(__m256i& acc, const int8_t* p) {
    __m256i v  = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(p));
    __m256i lo = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(v));
    __m256i hi = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(v, 1));
    acc = _mm256_add_epi32(acc, _mm256_madd_epi16(lo, lo));
    acc = _mm256_add_epi32(acc, _mm256_madd_epi16(hi, hi));
}

static float vec1(const int8_t* a, int N) {
    __m256i acc = _mm256_setzero_si256();
    for (int i = 0; i < N; i += 32)
        accumulate(acc, a + i);
    return sqrtOfSum(hsumEpi32(acc));
}

static float vec2(const int8_t* a, int N) {
    __m256i acc0 = _mm256_setzero_si256(), acc1 = _mm256_setzero_si256();
    for (int i = 0; i < N; i += 64) {
        accumulate(acc0, a + i);
        accumulate(acc1, a + i + 32);
    }
    return sqrtOfSum(hsumEpi32(_mm256_add_epi32(acc0, acc1)));
}

static float vec4(const int8_t* a, int N) {
    __m256i acc0 = _mm256_setzero_si256(), acc1 = _mm256_setzero_si256();
    __m256i acc2 = _mm256_setzero_si256(), acc3 = _mm256_setzero_si256();
    for (int i = 0; i < N; i += 128) {
        accumulate(acc0, a + i);
        accumulate(acc1, a + i + 32);
        accumulate(acc2, a + i + 64);
        accumulate(acc3, a + i + 96);
    }
    __m256i acc = _mm256_add_epi32(_mm256_add_epi32(acc0, acc1), _mm256_add_epi32(acc2, acc3));
    return sqrtOfSum(hsumEpi32(acc));
}

static float vec8(const int8_t* a, int N) {
    __m256i acc0 = _mm256_setzero_si256(), acc1 = _mm256_setzero_si256();
    __m256i acc2 = _mm256_setzero_si256(), acc3 = _mm256_setzero_si256();
    __m256i acc4 = _mm256_setzero_si256(), acc5 = _mm256_setzero_si256();
    __m256i acc6 = _mm256_setzero_si256(), acc7 = _mm256_setzero_si256();
    for (int i = 0; i < N; i += 256) {
        accumulate(acc0, a + i);
        accumulate(acc1, a + i + 32);
        accumulate(acc2, a + i + 64);
        accumulate(acc3, a + i + 96);
        accumulate(acc4, a + i + 128);
        accumulate(acc5, a + i + 160);
        accumulate(acc6, a + i + 192);
        accumulate(acc7, a + i + 224);
    }
    __m256i acc = _mm256_add_epi32(
        _mm256_add_epi32(_mm256_add_epi32(acc0, acc1), _mm256_add_epi32(acc2, acc3)),
        _mm256_add_epi32(_mm256_add_epi32(acc4, acc5), _mm256_add_epi32(acc6, acc7))
    );
    return sqrtOfSum(hsumEpi32(acc));
}

float normVec(const int8_t* a, int N, int M) {
    switch (M) {
        case 2:  return vec2(a, N);
        case 4:  return vec4(a, N);
        case 8:  return vec8(a, N);
        default: return vec1(a, N);
    }
}
