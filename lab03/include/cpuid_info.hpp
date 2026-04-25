#ifndef CPUID_INFO_HPP
#define CPUID_INFO_HPP

#include <array>
#include <cpuid.h>

using Regs = std::array<unsigned int, 4>; // EAX, EBX, ECX, EDX

inline Regs cpuid(unsigned int leaf, unsigned int subleaf = 0) {
    Regs r{};
    __cpuid_count(leaf, subleaf, r[0], r[1], r[2], r[3]);
    return r;
}

inline bool bit(unsigned int v, int pos) {
    return (v >> pos) & 1u;
}

inline unsigned int field(unsigned int v, int lo, int len) {
    return (v >> lo) & ((1u << len) - 1u);
}

#endif
