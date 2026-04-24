#ifndef UTILS_HPP
#define UTILS_HPP


float** createMatrix(int N);
float** createRandomMatrix(int N);
float** createZeroMatrix(int N);
void deleteMatrix(float** M, int N);

void fillRandom(float** M, int N);
void fillZero(float** M, int N);

bool compareMatrices(float** A, float** B, int N, float eps = 1e-3f);

void transposeMatrix(float** src, float** dst, int N);


inline float dot1(const float* A, const float* B, int N) {
    float s = 0;
    for (int k = 0; k < N; k++)
        s += A[k] * B[k];
    return s;
}

inline float dot2(const float* A, const float* B, int N) {
    float s0 = 0, s1 = 0;
    for (int k = 0; k < N; k += 2) {
        s0 += A[k]   * B[k];
        s1 += A[k+1] * B[k+1];
    }
    return s0 + s1;
}

inline float dot4(const float* A, const float* B, int N) {
    float s0 = 0, s1 = 0, s2 = 0, s3 = 0;
    for (int k = 0; k < N; k += 4) {
        s0 += A[k]   * B[k];
        s1 += A[k+1] * B[k+1];
        s2 += A[k+2] * B[k+2];
        s3 += A[k+3] * B[k+3];
    }
    return s0 + s1 + s2 + s3;
}

inline float dot8(const float* A, const float* B, int N) {
    float s0 = 0, s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0;
    for (int k = 0; k < N; k += 8) {
        s0 += A[k]   * B[k];
        s1 += A[k+1] * B[k+1];
        s2 += A[k+2] * B[k+2];
        s3 += A[k+3] * B[k+3];
        s4 += A[k+4] * B[k+4];
        s5 += A[k+5] * B[k+5];
        s6 += A[k+6] * B[k+6];
        s7 += A[k+7] * B[k+7];
    }
    return s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7;
}

inline float dot16(const float* A, const float* B, int N) {
    float s0  = 0, s1  = 0, s2  = 0, s3  = 0,
          s4  = 0, s5  = 0, s6  = 0, s7  = 0,
          s8  = 0, s9  = 0, s10 = 0, s11 = 0,
          s12 = 0, s13 = 0, s14 = 0, s15 = 0;
    for (int k = 0; k < N; k += 16) {
        s0  += A[k]    * B[k];
        s1  += A[k+1]  * B[k+1];
        s2  += A[k+2]  * B[k+2];
        s3  += A[k+3]  * B[k+3];
        s4  += A[k+4]  * B[k+4];
        s5  += A[k+5]  * B[k+5];
        s6  += A[k+6]  * B[k+6];
        s7  += A[k+7]  * B[k+7];
        s8  += A[k+8]  * B[k+8];
        s9  += A[k+9]  * B[k+9];
        s10 += A[k+10] * B[k+10];
        s11 += A[k+11] * B[k+11];
        s12 += A[k+12] * B[k+12];
        s13 += A[k+13] * B[k+13];
        s14 += A[k+14] * B[k+14];
        s15 += A[k+15] * B[k+15];
    }
    return s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7
         + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15;
}

inline float dot32(const float* A, const float* B, int N) {
    float s0  = 0, s1  = 0, s2  = 0, s3  = 0, s4  = 0, s5  = 0, s6  = 0, s7  = 0,
          s8  = 0, s9  = 0, s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0,
          s16 = 0, s17 = 0, s18 = 0, s19 = 0, s20 = 0, s21 = 0, s22 = 0, s23 = 0,
          s24 = 0, s25 = 0, s26 = 0, s27 = 0, s28 = 0, s29 = 0, s30 = 0, s31 = 0;
    for (int k = 0; k < N; k += 32) {
        s0  += A[k]    * B[k];    s1  += A[k+1]  * B[k+1];
        s2  += A[k+2]  * B[k+2];  s3  += A[k+3]  * B[k+3];
        s4  += A[k+4]  * B[k+4];  s5  += A[k+5]  * B[k+5];
        s6  += A[k+6]  * B[k+6];  s7  += A[k+7]  * B[k+7];
        s8  += A[k+8]  * B[k+8];  s9  += A[k+9]  * B[k+9];
        s10 += A[k+10] * B[k+10]; s11 += A[k+11] * B[k+11];
        s12 += A[k+12] * B[k+12]; s13 += A[k+13] * B[k+13];
        s14 += A[k+14] * B[k+14]; s15 += A[k+15] * B[k+15];
        s16 += A[k+16] * B[k+16]; s17 += A[k+17] * B[k+17];
        s18 += A[k+18] * B[k+18]; s19 += A[k+19] * B[k+19];
        s20 += A[k+20] * B[k+20]; s21 += A[k+21] * B[k+21];
        s22 += A[k+22] * B[k+22]; s23 += A[k+23] * B[k+23];
        s24 += A[k+24] * B[k+24]; s25 += A[k+25] * B[k+25];
        s26 += A[k+26] * B[k+26]; s27 += A[k+27] * B[k+27];
        s28 += A[k+28] * B[k+28]; s29 += A[k+29] * B[k+29];
        s30 += A[k+30] * B[k+30]; s31 += A[k+31] * B[k+31];
    }
    return s0+s1+s2+s3+s4+s5+s6+s7+s8+s9+s10+s11+s12+s13+s14+s15
          +s16+s17+s18+s19+s20+s21+s22+s23+s24+s25+s26+s27+s28+s29+s30+s31;
}

inline float dotM(const float* A, const float* B, int N, int M) {
    switch (M) {
        case 2:  return dot2(A, B, N);
        case 4:  return dot4(A, B, N);
        case 8:  return dot8(A, B, N);
        case 16: return dot16(A, B, N);
        case 32: return dot32(A, B, N);
        default: return dot1(A, B, N);
    }
}

#endif
