#include <stdio.h>

int snprintf(char* buffer, size_t n, const char* format, ...)
{
	va_list args = {};
	va_start(args, format);
	int ret = vsnprintf(buffer, n, format, args);
	va_end(args);

	return ret;
}

int sprintf(char* buffer, const char* format, ...)
{
	va_list args = {};
	va_start(args, format);
	int ret = vsprintf(buffer, format, args);
	va_end(args);

	return ret;
}

int fprintf(FILE* fp, const char* format, ...)
{
	va_list args = {};
	va_start(args, format);
	int ret = vfprintf(fp, format, args);
	va_end(args);

	return ret;
}

int printf(const char* format, ...)
{
	va_list args = {};
	va_start(args, format);
	int ret = vprintf(format, args);
	va_end(args);

	return ret;
}