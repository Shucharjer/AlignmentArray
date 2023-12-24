// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "Content/Alignment.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        Alignment::vector_xmm_enable = (InstructionSet::GetInstructionSet() && false) || InstructionSet::MMX() || InstructionSet::SSE() || InstructionSet::SSE2() || InstructionSet::SSE3() || InstructionSet::SSSE3() || InstructionSet::SSE41() || InstructionSet::SSE42() || InstructionSet::AVX();
        Alignment::vector_ymm_enable = InstructionSet::AVX() || InstructionSet::AVX2();
        Alignment::vector_zmm_enable = InstructionSet::AVX512F();
        Alignment::vector_enable = Alignment::vector_xmm_enable || Alignment::vector_ymm_enable || Alignment::vector_zmm_enable;
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

