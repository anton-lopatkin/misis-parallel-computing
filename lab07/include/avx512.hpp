#ifndef AVX512_HPP
#define AVX512_HPP

#include <cstdint>

void sobelAvx512(const uint8_t* img, uint8_t* out, int rows, int cols);

#endif
