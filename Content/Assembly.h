#pragma once
#include "Definations.h"

namespace Assembly
{
	extern "C" {
		ALIGNMENT_API void SwapX(void* ptr_1, void* ptr_2);
		ALIGNMENT_API void SwapY(void* ptr_1, void* ptr_2);
		ALIGNMENT_API void SwapZ(void* ptr_1, void* ptr_2);
		ALIGNMENT_API void CopyX(void* dst, void* src, size_t r_size);
		ALIGNMENT_API void CopyY(void* dst, void* src, size_t r_size);
		ALIGNMENT_API void CopyZ(void* dst, void* src, size_t r_size);
		ALIGNMENT_API void ZeroX(void* dst, size_t r_size);
		ALIGNMENT_API void ZeroY(void* dst, size_t r_size);
		ALIGNMENT_API void ZeroZ(void* dst, size_t r_size);
	}
}
