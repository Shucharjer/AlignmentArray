#include "InstructionSet.h"

ALIGNMENT_API int InstructionSet::nIds_;
ALIGNMENT_API int InstructionSet::nExIds_;
ALIGNMENT_API std::string InstructionSet::vendor_;
ALIGNMENT_API std::string InstructionSet::brand_;
ALIGNMENT_API bool InstructionSet::isIntel_;
ALIGNMENT_API bool InstructionSet::isAMD_;
ALIGNMENT_API std::bitset<32> InstructionSet::f_1_ECX_;
ALIGNMENT_API std::bitset<32> InstructionSet::f_1_EDX_;
ALIGNMENT_API std::bitset<32> InstructionSet::f_7_EBX_;
ALIGNMENT_API std::bitset<32> InstructionSet::f_7_ECX_;
ALIGNMENT_API std::bitset<32> InstructionSet::f_81_ECX_;
ALIGNMENT_API std::bitset<32> InstructionSet::f_81_EDX_;
ALIGNMENT_API std::vector<std::array<int, 4>> InstructionSet::data_;
ALIGNMENT_API std::vector<std::array<int, 4>> InstructionSet::extdata_;

bool InstructionSet::GetInstructionSet()
{
    std::array<int, 4> cpuid{ };

    __cpuid(cpuid.data(), 0);
    nIds_ = cpuid[0];

    for (int i = 0; i <= nIds_; ++i)
    {
        __cpuidex(cpuid.data(), i, 0);
        data_.push_back(cpuid);
    }

    char vendor[0x20];
    memset(vendor, 0, sizeof(vendor));
    *reinterpret_cast<int*>(vendor) = data_[0][1];
    *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
    *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
    vendor_ = vendor;
    if (vendor_ == "GenuineIntel")
    {
        InstructionSet::isIntel_ = true;
    }
    else if (vendor_ == "AuthenticAMD")
    {
        isAMD_ = true;
    }

    // load bitset with flags for function 0x00000001
    if (nIds_ >= 1)
    {
        f_1_ECX_ = data_[1][2];
        f_1_EDX_ = data_[1][3];
    }

    // load bitset with flags for function 0x00000007
    if (nIds_ >= 7)
    {
        f_7_EBX_ = data_[7][1];
        f_7_ECX_ = data_[7][2];
    }

    // Calling __cpuid with 0x80000000 as the function_id argument
    // gets the number of the highest valid extended ID.
    __cpuid(cpuid.data(), 0x80000000);
    nExIds_ = cpuid[0];

    char brand[0x40];
    memset(brand, 0, sizeof(brand));

    for (int i = 0x80000000; i <= nExIds_; ++i)
    {
        __cpuidex(cpuid.data(), i, 0);
        extdata_.push_back(cpuid);
    }

    // load bitset with flags for function 0x80000001
    if (nExIds_ >= 0x80000001)
    {
        f_81_ECX_ = extdata_[1][2];
        f_81_EDX_ = extdata_[1][3];
    }

    // Interpret CPU brand string if reported
    if (nExIds_ >= 0x80000004)
    {
        memcpy(brand, extdata_[2].data(), sizeof(cpuid));
        memcpy(brand + 16, extdata_[3].data(), sizeof(cpuid));
        memcpy(brand + 32, extdata_[4].data(), sizeof(cpuid));
        brand_ = brand;
    }
    return true;
}
ALIGNMENT_API std::string InstructionSet::Vendor() { return vendor_; }
ALIGNMENT_API std::string InstructionSet::Brand() { return brand_; }

ALIGNMENT_API bool InstructionSet::SSE3(void) { return f_1_ECX_[0]; }
ALIGNMENT_API bool InstructionSet::PCLMULQDQ(void) { return f_1_ECX_[1]; }
ALIGNMENT_API bool InstructionSet::MONITOR(void) { return f_1_ECX_[3]; }
ALIGNMENT_API bool InstructionSet::SSSE3(void) { return f_1_ECX_[9]; }
ALIGNMENT_API bool InstructionSet::FMA(void) { return f_1_ECX_[12]; }
ALIGNMENT_API bool InstructionSet::CMPXCHG16B(void) { return f_1_ECX_[13]; }
ALIGNMENT_API bool InstructionSet::SSE41(void) { return f_1_ECX_[19]; }
ALIGNMENT_API bool InstructionSet::SSE42(void) { return f_1_ECX_[20]; }
ALIGNMENT_API bool InstructionSet::MOVBE(void) { return f_1_ECX_[22]; }
ALIGNMENT_API bool InstructionSet::POPCNT(void) { return f_1_ECX_[23]; }
ALIGNMENT_API bool InstructionSet::AES(void) { return f_1_ECX_[25]; }
ALIGNMENT_API bool InstructionSet::XSAVE(void) { return f_1_ECX_[26]; }
ALIGNMENT_API bool InstructionSet::OSXSAVE(void) { return f_1_ECX_[27]; }
ALIGNMENT_API bool InstructionSet::AVX(void) { return f_1_ECX_[28]; }
ALIGNMENT_API bool InstructionSet::F16C(void) { return f_1_ECX_[29]; }
ALIGNMENT_API bool InstructionSet::RDRAND(void) { return f_1_ECX_[30]; }

ALIGNMENT_API bool InstructionSet::MSR(void) { return f_1_EDX_[5]; }
ALIGNMENT_API bool InstructionSet::CX8(void) { return f_1_EDX_[8]; }
ALIGNMENT_API bool InstructionSet::SEP(void) { return f_1_EDX_[11]; }
ALIGNMENT_API bool InstructionSet::CMOV(void) { return f_1_EDX_[15]; }
ALIGNMENT_API bool InstructionSet::CLFSH(void) { return f_1_EDX_[19]; }
ALIGNMENT_API bool InstructionSet::MMX(void) { return f_1_EDX_[23]; }
ALIGNMENT_API bool InstructionSet::FXSR(void) { return f_1_EDX_[24]; }
ALIGNMENT_API bool InstructionSet::SSE(void) { return f_1_EDX_[25]; }
ALIGNMENT_API bool InstructionSet::SSE2(void) { return f_1_EDX_[26]; }

ALIGNMENT_API bool InstructionSet::FSGSBASE(void) { return f_7_EBX_[0]; }
ALIGNMENT_API bool InstructionSet::BMI1(void) { return f_7_EBX_[3]; }
ALIGNMENT_API bool InstructionSet::HLE(void) { return isIntel_ && f_7_EBX_[4]; }
ALIGNMENT_API bool InstructionSet::AVX2(void) { return f_7_EBX_[5]; }
ALIGNMENT_API bool InstructionSet::BMI2(void) { return f_7_EBX_[8]; }
ALIGNMENT_API bool InstructionSet::ERMS(void) { return f_7_EBX_[9]; }
ALIGNMENT_API bool InstructionSet::INVPCID(void) { return f_7_EBX_[10]; }
ALIGNMENT_API bool InstructionSet::RTM(void) { return isIntel_ && f_7_EBX_[11]; }
ALIGNMENT_API bool InstructionSet::AVX512F(void) { return f_7_EBX_[16]; }
ALIGNMENT_API bool InstructionSet::RDSEED(void) { return f_7_EBX_[18]; }
ALIGNMENT_API bool InstructionSet::ADX(void) { return f_7_EBX_[19]; }
ALIGNMENT_API bool InstructionSet::AVX512PF(void) { return f_7_EBX_[26]; }
ALIGNMENT_API bool InstructionSet::AVX512ER(void) { return f_7_EBX_[27]; }
ALIGNMENT_API bool InstructionSet::AVX512CD(void) { return f_7_EBX_[28]; }
ALIGNMENT_API bool InstructionSet::SHA(void) { return f_7_EBX_[29]; }

ALIGNMENT_API bool InstructionSet::PREFETCHWT1(void) { return f_7_ECX_[0]; }

ALIGNMENT_API bool InstructionSet::LAHF(void) { return f_81_ECX_[0]; }
ALIGNMENT_API bool InstructionSet::LZCNT(void) { return isIntel_ && f_81_ECX_[5]; }
ALIGNMENT_API bool InstructionSet::ABM(void) { return isAMD_ && f_81_ECX_[5]; }
ALIGNMENT_API bool InstructionSet::SSE4a(void) { return isAMD_ && f_81_ECX_[6]; }
ALIGNMENT_API bool InstructionSet::XOP(void) { return isAMD_ && f_81_ECX_[11]; }
ALIGNMENT_API bool InstructionSet::TBM(void) { return isAMD_ && f_81_ECX_[21]; }

ALIGNMENT_API bool InstructionSet::SYSCALL(void) { return isIntel_ && f_81_EDX_[11]; }
ALIGNMENT_API bool InstructionSet::MMXEXT(void) { return isAMD_ && f_81_EDX_[22]; }
ALIGNMENT_API bool InstructionSet::RDTSCP(void) { return isIntel_ && f_81_EDX_[27]; }
ALIGNMENT_API bool InstructionSet::_3DNOWEXT(void) { return isAMD_ && f_81_EDX_[30]; }
ALIGNMENT_API bool InstructionSet::_3DNOW(void) { return isAMD_ && f_81_EDX_[31]; }