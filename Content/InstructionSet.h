#pragma once
#include "Definations.h"
#include <string>
#include <bitset>
#include <intrin.h>
#include <array>
#include <vector>

namespace InstructionSet
{
    ALIGNMENT_API extern int nIds_;
    ALIGNMENT_API extern int nExIds_;
    ALIGNMENT_API extern std::string vendor_;
    ALIGNMENT_API extern std::string brand_;
    ALIGNMENT_API extern bool isIntel_;
    ALIGNMENT_API extern bool isAMD_;
    ALIGNMENT_API extern std::bitset<32> f_1_ECX_;
    ALIGNMENT_API extern std::bitset<32> f_1_EDX_;
    ALIGNMENT_API extern std::bitset<32> f_7_EBX_;
    ALIGNMENT_API extern std::bitset<32> f_7_ECX_;
    ALIGNMENT_API extern std::bitset<32> f_81_ECX_;
    ALIGNMENT_API extern std::bitset<32> f_81_EDX_;
    ALIGNMENT_API extern std::vector<std::array<int, 4>> data_;
    ALIGNMENT_API extern std::vector<std::array<int, 4>> extdata_;

    ALIGNMENT_API bool GetInstructionSet();
    ALIGNMENT_API std::string Vendor();
    ALIGNMENT_API std::string Brand();

    ALIGNMENT_API bool SSE3(void);
    ALIGNMENT_API bool PCLMULQDQ(void);
    ALIGNMENT_API bool MONITOR(void);
    ALIGNMENT_API bool SSSE3(void);
    ALIGNMENT_API bool FMA(void);
    ALIGNMENT_API bool CMPXCHG16B(void);
    ALIGNMENT_API bool SSE41(void);
    ALIGNMENT_API bool SSE42(void);
    ALIGNMENT_API bool MOVBE(void);
    ALIGNMENT_API bool POPCNT(void);
    ALIGNMENT_API bool AES(void);
    ALIGNMENT_API bool XSAVE(void);
    ALIGNMENT_API bool OSXSAVE(void);
    ALIGNMENT_API bool AVX(void);
    ALIGNMENT_API bool F16C(void);
    ALIGNMENT_API bool RDRAND(void);

    ALIGNMENT_API bool MSR(void);
    ALIGNMENT_API bool CX8(void);
    ALIGNMENT_API bool SEP(void);
    ALIGNMENT_API bool CMOV(void);
    ALIGNMENT_API bool CLFSH(void);
    ALIGNMENT_API bool MMX(void);
    ALIGNMENT_API bool FXSR(void);
    ALIGNMENT_API bool SSE(void);
    ALIGNMENT_API bool SSE2(void);

    ALIGNMENT_API bool FSGSBASE(void);
    ALIGNMENT_API bool BMI1(void);
    ALIGNMENT_API bool HLE(void);
    ALIGNMENT_API bool AVX2(void);
    ALIGNMENT_API bool BMI2(void);
    ALIGNMENT_API bool ERMS(void);
    ALIGNMENT_API bool INVPCID(void);
    ALIGNMENT_API bool RTM(void);
    ALIGNMENT_API bool AVX512F(void);
    ALIGNMENT_API bool RDSEED(void);
    ALIGNMENT_API bool ADX(void);
    ALIGNMENT_API bool AVX512PF(void);
    ALIGNMENT_API bool AVX512ER(void);
    ALIGNMENT_API bool AVX512CD(void);
    ALIGNMENT_API bool SHA(void);

    ALIGNMENT_API bool PREFETCHWT1(void);

    ALIGNMENT_API bool LAHF(void);
    ALIGNMENT_API bool LZCNT(void);
    ALIGNMENT_API bool ABM(void);
    ALIGNMENT_API bool SSE4a(void);
    ALIGNMENT_API bool XOP(void);
    ALIGNMENT_API bool TBM(void);

    ALIGNMENT_API bool SYSCALL(void);
    ALIGNMENT_API bool MMXEXT(void);
    ALIGNMENT_API bool RDTSCP(void);
    ALIGNMENT_API bool _3DNOWEXT(void);
    ALIGNMENT_API bool _3DNOW(void);
}