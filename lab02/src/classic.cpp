#include "classic.hpp"

void multiplyClassic(float** A, float** B, float** C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            float s = 0.0f;
            for (int k = 0; k < N; k++) {
                s += A[i][k] * B[k][j];
            }
            C[i][j] = s;
        }
    }
}