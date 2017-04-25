#pragma once

#ifdef SH_BUILD
#define SHAPI __declspec(dllexport)
#else
#define SHAPI __declspec(dllimport)
#endif


