#include "avx512.hpp"
#include "naive.hpp"
#include <immintrin.h>

void sobelAvx512(const uint8_t* img, uint8_t* out, int rows, int cols) {
    const __m512 vNorm = _mm512_set1_ps(256.0f / 1140.0f);

    for (int y = 1; y < rows - 1; y++) {
        int x = 1;
        for (; x + 32 <= cols - 1; x += 32) {
            const uint8_t* rp = img + (y-1)*cols + x-1;
            const uint8_t* rm = img + y*cols + x-1;
            const uint8_t* rb = img + (y+1)*cols + x-1;

            __m256i A8 = _mm256_loadu_si256((const __m256i*)(rp));
            __m256i B8 = _mm256_loadu_si256((const __m256i*)(rp + 1));
            __m256i C8 = _mm256_loadu_si256((const __m256i*)(rp + 2));
            __m256i D8 = _mm256_loadu_si256((const __m256i*)(rm));
            __m256i F8 = _mm256_loadu_si256((const __m256i*)(rm + 2));
            __m256i G8 = _mm256_loadu_si256((const __m256i*)(rb));
            __m256i H8 = _mm256_loadu_si256((const __m256i*)(rb + 1));
            __m256i I8 = _mm256_loadu_si256((const __m256i*)(rb + 2));

            __m512i vA = _mm512_cvtepu8_epi16(A8);
            __m512i vB = _mm512_cvtepu8_epi16(B8);
            __m512i vC = _mm512_cvtepu8_epi16(C8);
            __m512i vD = _mm512_cvtepu8_epi16(D8);
            __m512i vF = _mm512_cvtepu8_epi16(F8);
            __m512i vG = _mm512_cvtepu8_epi16(G8);
            __m512i vH = _mm512_cvtepu8_epi16(H8);
            __m512i vI = _mm512_cvtepu8_epi16(I8);

            __m512i vt1 = _mm512_sub_epi16(vA, vI);
            __m512i vt2 = _mm512_sub_epi16(vC, vG);

            __m512i vHh = _mm512_add_epi16(
                _mm512_slli_epi16(_mm512_sub_epi16(vD, vF), 1), 
                _mm512_sub_epi16(vt1, vt2)
            );
            __m512i vHv = _mm512_add_epi16(
                _mm512_slli_epi16(_mm512_sub_epi16(vB, vH), 1),
                _mm512_add_epi16(vt1, vt2)
            );

            __m512i Hh_lo = _mm512_cvtepi16_epi32(_mm512_castsi512_si256(vHh));
            __m512i Hh_hi = _mm512_cvtepi16_epi32(_mm512_extracti64x4_epi64(vHh, 1));
            __m512i Hv_lo = _mm512_cvtepi16_epi32(_mm512_castsi512_si256(vHv));
            __m512i Hv_hi = _mm512_cvtepi16_epi32(_mm512_extracti64x4_epi64(vHv, 1));

            __m512 fHh_lo = _mm512_cvtepi32_ps(Hh_lo);
            __m512 fHh_hi = _mm512_cvtepi32_ps(Hh_hi);
            __m512 fHv_lo = _mm512_cvtepi32_ps(Hv_lo);
            __m512 fHv_hi = _mm512_cvtepi32_ps(Hv_hi);

            __m512i di_lo = _mm512_cvttps_epi32(
                _mm512_mul_ps(
                    _mm512_sqrt_ps(
                        _mm512_add_ps(
                            _mm512_mul_ps(fHh_lo, fHh_lo),
                            _mm512_mul_ps(fHv_lo, fHv_lo)
                        )
                    ),
                    vNorm
                )
            );
            __m512i di_hi = _mm512_cvttps_epi32(
                _mm512_mul_ps(
                    _mm512_sqrt_ps(
                        _mm512_add_ps(
                            _mm512_mul_ps(fHh_hi, fHh_hi),
                            _mm512_mul_ps(fHv_hi, fHv_hi)
                        )
                    ),
                    vNorm
                )
            );

            __m256i lo16 = _mm512_cvtepi32_epi16(di_lo);
            __m256i hi16 = _mm512_cvtepi32_epi16(di_hi);
            __m512i d16  = _mm512_inserti64x4(_mm512_castsi256_si512(lo16), hi16, 1);
            __m256i d8   = _mm512_cvtusepi16_epi8(d16);

            _mm256_storeu_si256((__m256i*)(out + y*cols + x), d8);
        }
        for (; x < cols - 1; x++)
            sobelPixel(img, out, cols, x, y);
    }
}
