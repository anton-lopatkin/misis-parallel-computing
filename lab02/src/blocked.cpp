#include "blocked.hpp"
#include "utils.hpp"
#include <algorithm>

void multiplyBlocked(float** A, float** B, float** C, int N, int S, int M) {
    float* tmpA  = new float[S * S];
    float* tmpBT = new float[S * S];

    for (int ii = 0; ii < N; ii += S) {
        for (int jj = 0; jj < N; jj += S) {
            for (int kk = 0; kk < N; kk += S) {
                
                for (int i = 0; i < S; i++)
                    for (int k = 0; k < S; k++)
                        tmpA[i * S + k] = A[ii + i][kk + k];

                for (int j = 0; j < S; j++)
                    for (int k = 0; k < S; k++)
                        tmpBT[j * S + k] = B[kk + k][jj + j];

                for (int i = 0; i < S; i++)
                    for (int j = 0; j < S; j++)
                        C[ii + i][jj + j] += dotM(&tmpA[i * S], &tmpBT[j * S], S, M);
            }
        }
    }

    delete[] tmpA;
    delete[] tmpBT;
}
