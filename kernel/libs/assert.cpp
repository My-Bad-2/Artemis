#ifdef DEBUG

#include <assert.h>
#include <logger.hpp>

void __assert(const char* restrict e, const char* restrict file, int line)
{
	LogPanic("%s:%u: failed assertion `%s`", file, line, e);
}

#endif // DDEBUG