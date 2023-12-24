#pragma once

#ifdef ALIGNMENT_EXPORTS
#define ALIGNMENT_API __declspec(dllexport)
#else
#define ALIGNMENT_API __declspec(dllimport)
#pragma comment(lib, "Alignment.dll")
#endif