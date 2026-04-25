#include "utils.hpp"
#include <cstdlib>

int32_t* createArray(size_t N) {
    return new int32_t[N];
}

void deleteArray(int32_t* a) {
    delete[] a;
}

void fillRandom(int32_t* a, size_t N) {
    for (size_t i = 0; i < N; i++)
        a[i] = rand();
}

void buildIdx(int32_t* idx, int N) {
    for (int i = 0; i < N; i++)
        idx[i] = rand() % N;
}
