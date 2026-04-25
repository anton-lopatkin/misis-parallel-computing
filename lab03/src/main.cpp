#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <cstdint>

#include "cpuid_info.hpp"

static void section(const char* title) {
    std::cout << "\n" << title << "\n";
}

static void row(const char* name, unsigned int val, const char* unit = "") {
    std::cout << "  " << std::left << std::setw(32) << name << val;
    if (unit[0]) std::cout << " " << unit;
    std::cout << "\n";
}

static void flag(const char* name, bool val) {
    std::cout << "  " << std::left << std::setw(32) << name << (val ? "yes" : "no") << "\n";
}

static void hex32(const char* name, unsigned int val) {
    std::ostringstream s;
    s << "0x" << std::hex << std::setfill('0') << std::setw(8) << val;
    std::cout << "  " << std::left << std::setw(32) << name << s.str() << "\n";
}

int main() {
    Regs r0 = cpuid(0);
    unsigned int maxLeaf = r0[0];

    char vendor[13] = {};
    std::memcpy(vendor + 0, &r0[1], 4);
    std::memcpy(vendor + 4, &r0[3], 4);
    std::memcpy(vendor + 8, &r0[2], 4);

    section("EAX=0");
    row("Max basic function", maxLeaf);
    std::cout << "  " << std::left << std::setw(32) << "Vendor" << vendor << "\n";

    Regs r1 = cpuid(1);
    unsigned int eax = r1[0], ebx = r1[1], ecx = r1[2], edx = r1[3];

    section("EAX=1: Processor Info");
    hex32("EAX", eax);
    row("  Stepping ID", field(eax, 0, 4));
    row("  Model", field(eax, 4, 4));
    row("  Family ID", field(eax, 8, 4));
    row("  Processor type", field(eax, 12, 2));
    row("  Extended model", field(eax, 16, 4));
    row("  Extended family", field(eax, 20, 8));

    hex32("EBX", ebx);
    row("  Max logical processors", field(ebx, 16, 8));
    row("  Local APIC ID", field(ebx, 24, 8));

    hex32("EDX", edx);
    flag("  FPU", bit(edx, 0));
    flag("  TSC", bit(edx, 4));
    flag("  MMX", bit(edx, 23));
    flag("  SSE", bit(edx, 25));
    flag("  SSE2", bit(edx, 26));
    flag("  HTT", bit(edx, 28));

    hex32("ECX", ecx);
    flag("  SSE3", bit(ecx, 0));
    flag("  SSSE3", bit(ecx, 9));
    flag("  FMA3", bit(ecx, 12));
    flag("  SSE4.1", bit(ecx, 19));
    flag("  SSE4.2", bit(ecx, 20));
    flag("  AVX", bit(ecx, 28));

    const char* cacheTypes[] = {"(none)", "Data", "Instruction", "Unified"};

    section("Cache Info");
    for (unsigned int sub = 0; ; ++sub) {
        Regs rc = cpuid(4, sub);
        unsigned int ctype = field(rc[0], 0, 5);
        if (ctype == 0) break;

        unsigned int level = field(rc[0], 5, 3);
        bool fa = bit(rc[0], 9);
        unsigned int threads = field(rc[0], 14, 12) + 1;
        unsigned int cores = field(rc[0], 26, 6) + 1;
        unsigned int lsize = field(rc[1], 0, 12) + 1;
        unsigned int parts = field(rc[1], 12, 10) + 1;
        unsigned int ways = field(rc[1], 22, 10) + 1;
        unsigned int sets = rc[2] + 1;
        bool incl = bit(rc[3], 1);
        unsigned int sizeKB = (unsigned int)((uint64_t)lsize * parts * ways * sets / 1024);

        const char* tname;
        if (ctype < 4) tname = cacheTypes[ctype];
        else tname = "?";

        std::cout << "\n  L" << level << " " << tname << " cache:\n";
        row("    Cache line size", lsize, "Bytes");
        row("    Line partitions", parts);
        row("    Associativity", ways);
        row("    Sets", sets);
        row("    Threads per cache", threads);
        row("    Processor cores", cores);
        row("    Cache size", sizeKB, "KB");
        flag("    Fully associative", fa);
        flag("    Inclusive", incl);
    }

    Regs r70 = cpuid(7, 0);
    unsigned int ebx7 = r70[1], ecx7 = r70[2], edx7 = r70[3];

    section("EAX=7, ECX=0: Extended Features");
    flag("AVX2", bit(ebx7, 5));
    flag("RTM/TSX", bit(ebx7, 11));
    flag("AVX512-F", bit(ebx7, 16));
    flag("SHA", bit(ebx7, 29));
    flag("GFNI", bit(ecx7, 8));
    flag("AMX-BF16", bit(edx7, 22));
    flag("AMX-Tile", bit(edx7, 24));
    flag("AMX-INT8", bit(edx7, 25));

    Regs r71 = cpuid(7, 1);
    section("EAX=7, ECX=1: Extended Features");
    flag("AMX-Complex", bit(r71[3], 8));
    flag("AVX10", bit(r71[3], 19));

    Regs r16 = cpuid(0x16);
    section("EAX=16h: Frequencies");
    row("Base frequency", r16[0] & 0xFFFF, "MHz");
    row("Max frequency", r16[1] & 0xFFFF, "MHz");
    row("Bus frequency", r16[2] & 0xFFFF, "MHz");

    Regs re0 = cpuid(0x80000000u);
    unsigned int maxExt = re0[0];

    section("EAX=80000000h");
    hex32("Max extended function", maxExt);

    Regs re1 = cpuid(0x80000001u);
    section("EAX=80000001h: Extended CPU Features");
    flag("SSE4a", bit(re1[2], 6));
    flag("FMA4", bit(re1[2], 16));
    flag("3DNow!", bit(re1[3], 31));
    flag("Ext 3DNow!", bit(re1[3], 30));

    char brand[49] = {};
    for (int i = 0; i < 3; ++i) {
        Regs rb = cpuid(0x80000002u + i);
        std::memcpy(brand + i * 16, rb.data(), 16);
    }
    section("EAX=80000002h-80000004h: CPU Brand String");
    std::cout << "  " << brand << "\n";

    return 0;
}
