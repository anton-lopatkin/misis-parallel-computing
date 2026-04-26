#ifndef NAIVE_HPP
#define NAIVE_HPP

#include <cstdint>

void sobelPixel(const uint8_t* img, uint8_t* out, int cols, int x, int y);
void sobelNaive(const uint8_t* img, uint8_t* out, int cols, int rows);

#endif
