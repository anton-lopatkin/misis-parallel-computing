#include "buffered.hpp"
#include "utils.hpp"

void multiplyBuffered(float** A, float** B, float** C, int N, int M) {
    float* tmp = new float[N];

    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) tmp[k] = B[k][j];

        for (int i = 0; i < N; i++)
            C[i][j] = dotM(A[i], tmp, N, M);
    }

    delete[] tmp;
}
