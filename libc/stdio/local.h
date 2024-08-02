#pragma once

#include <stdint.h>
#include <stdio.h>

#define STDIO_INTEGER_BUFFER_SIZE 32
#define STDIO_LOG10_TAYLOR_TERMS 4

#define STDIO_ABS(x) ((size_t)((x) > 0 ? (x) : -((long long)x)))

enum StdioFlags : uint32_t
{
	FlagZeropad = (1 << 0),
	FlagLeft = (1 << 1),
	FlagPlus = (1 << 2),
	FlagSpace = (1 << 3),
	FlagHash = (1 << 4),
	FlagUppercase = (1 << 5),
	FlagChar = (1 << 6),
	FlagShort = (1 << 7),
	FlagInt = (1 << 8),
	FlagLong = (1 << 9),
	FlagLongLong = (1 << 10),
	FlagPrecision = (1 << 11),
	FlagAdaptExp = (1 << 12),
	FlagPointer = (1 << 13),
	FlagSigned = (1 << 14),
	FlagInt8 = FlagChar,

#if(SHRT_MAX == 32767LL)
	FlagInt16 = FlagShort,
#elif(INT_MAX == 32767LL)
	FlagInt16 = FlagInt,
#elif(LONG_MAX == 32767LL)
	FlagInt16 = FlagLong,
#elif(LLONG_MAX == 32767LL)
	FlagInt16 = FlagLongLong,
#else
	#error "No basic integer type has a size of 16 bits exactly"
#endif

#if(SHRT_MAX == 2147483647LL)
	FlagInt32 = FLAGS_SHORT,
#elif(INT_MAX == 2147483647LL)
	FlagInt32 = FlagInt,
#elif(LONG_MAX == 2147483647LL)
	FlagInt32 = FlagLong,
#elif(LLONG_MAX == 2147483647LL)
	FlagInt32 = FlagLongLong,
#else
	#error "No basic integer type has a size of 32 bits exactly"
#endif

#if(SHRT_MAX == 9223372036854775807LL)
	FlagInt64 = FLAGS_SHORT,
#elif(INT_MAX == 9223372036854775807LL)
	FlagInt64 = FlagInt,
#elif(LONG_MAX == 9223372036854775807LL)
	FlagInt64 = FlagLong,
#elif(LLONG_MAX == 9223372036854775807LL)
	FlagInt64 = FlagLongLong,
#else
	#error "No basic integer type has a size of 64 bits exactly"
#endif
};

enum StdioBase
{
	BaseBinary = 2,
	BaseOctal = 8,
	BaseDecimal = 10,
	BaseHex = 16,
};

typedef uint32_t stdio_flags_t;

int StdioVsnprintf(FILE* fp, const char* format, va_list args);