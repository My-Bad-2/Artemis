#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include <sys/defs.h>

#define putc(ch, fp) fputc(ch, fp)
#define BUFSIZE INT_MAX

#ifdef __cplusplus
	#define FILE_FP FILE
#else
	#define FILE_FP __file
#endif

struct FILE_FP
{
	char* buffer;
	size_t position;
	size_t max_chars;
	void* args;
	void (*write)(int c, void* args);

#ifdef __cplusplus
	constexpr FILE_FP() : buffer(nullptr), position(0), max_chars(0), args(nullptr), write(nullptr)
	{
	}

	FILE_FP(char* buffer, size_t buffer_size) :
		buffer(nullptr), position(0), max_chars(0), args(nullptr), write(nullptr)
	{
		size_t usable_buffer_size = (buffer_size > BUFSIZE) ? BUFSIZE : buffer_size;

		if(buffer != nullptr)
		{
			this->buffer = buffer;
			this->max_chars = usable_buffer_size;
		}
	}

	FILE_FP(void (*func)(int, void*), void* args) :
		buffer(nullptr), position(0), max_chars(BUFSIZE), args(args), write(func)
	{
	}
#endif
};

#ifndef __cplusplus
typedef struct __file FILE;
#endif

__CDECLS_BEGIN

extern FILE stdio_streams[3];

#define stdin (&stdio_streams[0])
#define stdout (&stdio_streams[1])
#define stderr (&stdio_streams[2])

int fputc(int ch, FILE* fp);
int fputs(const char* restrict str, FILE* fp);

int putchar(int ch);
int puts(const char* restrict str);

int snprintf(char* buffer, size_t n, const char* format, ...) __PRINTFLIKE(3, 4);
int sprintf(char* buffer, const char* format, ...) __PRINTFLIKE(2, 3);
int fprintf(FILE* fp, const char* format, ...) __PRINTFLIKE(2, 3);
int printf(const char* format, ...) __PRINTFLIKE(1, 2);

int vsnprintf(char* buffer, size_t n, const char* format, va_list args) __PRINTFLIKE(3, 0);
int vsprintf(char* buffer, const char* format, va_list args) __PRINTFLIKE(2, 0);
int vfprintf(FILE* fp, const char* format, va_list args) __PRINTFLIKE(2, 0);
int vprintf(const char* format, va_list args) __PRINTFLIKE(1, 0);

__CDECLS_END

#endif