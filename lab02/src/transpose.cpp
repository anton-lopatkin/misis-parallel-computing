#include "transpose.hpp"
#include "utils.hpp"

void multiplyTransposedB(float** A, float** BT, float** C, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            float s = 0.0f;
            for (int k = 0; k < N; k++)
                s += A[i][k] * BT[j][k];
            C[i][j] = s;
        }
}

void multiplyTranspose(float** A, float** B, float** C, int N) {
    float** BT = createMatrix(N);
    transposeMatrix(B, BT, N);
    multiplyTransposedB(A, BT, C, N);
    deleteMatrix(BT, N);
}