#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <cstddef>

int32_t* createArray(size_t N);
void deleteArray(int32_t* a);

void fillRandom(int32_t* a, size_t N);
void buildIdx(int32_t* idx, int N);

#endif
