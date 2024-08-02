#include "local.h"

int vsnprintf(char* buffer, size_t n, const char* format, va_list args)
{
	FILE fp = FILE(buffer, n);
	return StdioVsnprintf(&fp, format, args);
}

int vsprintf(char* buffer, const char* format, va_list args)
{
	return vsnprintf(buffer, BUFSIZE, format, args);
}

int vfprintf(FILE* fp, const char* format, va_list args)
{
	return StdioVsnprintf(fp, format, args);
}

int vprintf(const char* format, va_list args)
{
	return vfprintf(stdout, format, args);
}