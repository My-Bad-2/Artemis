#ifndef _ASSERT_H
#define _ASSERT_H

#include <sys/defs.h>

// Refer to: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2264r7.html

#ifdef DEBUG

__CDECLS_BEGIN

void __assert(const char* restrict e, const char* restrict file, int line);

__CDECLS_END

	#ifdef __cplusplus
		#define assert(...)                                             \
			((void)((__VA_ARGS__) ? ((void)sizeof(bool(__VA_ARGS__))) : \
									__assert(#__VA_ARGS__, __FILE__, __LINE__)))
	#else
static inline int __check_single_argument_passed_to_assert(int b)
{
	return b;
}

		#define assert(...)                                                 \
			((void)(__check_single_argument_passed_to_assert(__VA_ARGS__) ? \
						((void)0) :                                         \
						__assert(#_VA_ARGS__, __FILE__, __LINE__)))

	#endif

#else // DEBUG
	#define assert(...) ((void)0)
#endif // DEBUG

#endif // _ASSERT_H