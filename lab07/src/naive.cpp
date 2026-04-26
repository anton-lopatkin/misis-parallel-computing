#include "naive.hpp"
#include <cmath>

void sobelPixel(const uint8_t* img, uint8_t* out, int cols, int x, int y) {
    int A = img[(y-1)*cols + x-1];
    int B = img[(y-1)*cols + x];
    int C = img[(y-1)*cols + x+1];
    int D = img[y*cols + x-1];
    int F = img[y*cols + x+1];
    int G = img[(y+1)*cols + x-1];
    int H = img[(y+1)*cols + x];
    int I = img[(y+1)*cols + x+1];
    int t1 = A - I;
    int t2 = C - G;
    int Hh = 2 * (D - F) + t1 - t2;
    int Hv = 2 * (B - H) + t1 + t2;
    int d = (int)(256.0f / 1140.0f * sqrtf((float)(Hh * Hh + Hv * Hv)));
    out[y*cols + x] = (uint8_t)(d > 255 ? 255 : d);
}

void sobelNaive(const uint8_t* img, uint8_t* out, int cols, int rows) {
    for (int y = 1; y < rows - 1; y++)
        for (int x = 1; x < cols - 1; x++)
            sobelPixel(img, out, cols, x, y);
}
