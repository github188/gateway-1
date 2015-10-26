#ifndef __TRADE_COMMON_H__
#define __TRADE_COMMON_H__

#include <string.h>

#ifdef _WIN32

#ifdef SDKDLL_EXPORTS
#define SDKDLL_API __declspec(dllexport)
#else
#define SDKDLL_API __declspec(dllimport)
#endif
#else
#define SDKDLL_API
#endif

#endif //__TRADE_COMMON_H__

