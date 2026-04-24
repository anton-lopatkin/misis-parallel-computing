#include "utils.hpp"
#include <cstdlib>
#include <cmath>

float** createMatrix(int N) {
    float** M = new float*[N];
    float* data = new float[N * N];
    for (int i = 0; i < N; i++)
        M[i] = data + i * N;
    return M;
}

void deleteMatrix(float** M, int N) {
    delete[] M[0];
    delete[] M;
}

void fillRandom(float** M, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M[i][j] = static_cast<float>(rand()) / RAND_MAX;
}

void fillZero(float** M, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M[i][j] = 0.0f;
}

float** createRandomMatrix(int N) {
    float** M = createMatrix(N);
    fillRandom(M, N);
    return M;
}

float** createZeroMatrix(int N) {
    float** M = createMatrix(N);
    fillZero(M, N);
    return M;
}

bool compareMatrices(float** A, float** B, int N, float eps) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (fabs(A[i][j] - B[i][j]) > eps)
                return false;
    return true;
}

void transposeMatrix(float** src, float** dst, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            dst[j][i] = src[i][j];
}
