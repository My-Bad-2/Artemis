#include "local.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

namespace details
{
inline size_t StdioAtou(const char** str)
{
	size_t i = 0;

	while(isdigit(**str))
	{
		i = i * 10 + (size_t)(*((*str)++) - '0');
	}

	return i;
}

inline void StdioAppendTermination(FILE* fp)
{
	if((fp->write != NULL) || (fp->max_chars == 0))
	{
		return;
	}

	if(fp->buffer == NULL)
	{
		return;
	}

	size_t null_pos = (fp->position < fp->max_chars) ? fp->position : fp->max_chars - 1;
	fp->buffer[null_pos] = '\0';
}

inline stdio_flags_t StdioParseFlags(const char** format)
{
	stdio_flags_t flags = 0;

	do
	{
		switch(**format)
		{
			case '0':
				flags |= FlagZeropad;
				(*format)++;
				break;
			case '-':
				flags |= FlagLeft;
				(*format)++;
				break;
			case '+':
				flags |= FlagPlus;
				(*format)++;
				break;
			case ' ':
				flags |= FlagSpace;
				(*format)++;
				break;
			case '#':
				flags |= FlagHash;
				(*format)++;
				break;
			default:
				return flags;
		}
	} while(true);
}

void StdioOutputReverse(FILE* fp, const char* buffer, size_t len, size_t width, stdio_flags_t flags)
{
	const size_t start_pos = fp->position;

	if(!(flags & FlagLeft) && !(flags & FlagZeropad))
	{
		for(size_t i = len; i < width; i++)
		{
			fputc(' ', fp);
		}
	}

	while(len)
	{
		fputc(buffer[--len], fp);
	}

	if(flags & FlagLeft)
	{
		while((fp->position - start_pos) < width)
		{
			fputc(' ', fp);
		}
	}
}

void StdioPrintInteger(FILE* fp, char* buffer, size_t len, bool negative, uint8_t base,
					   size_t precision, size_t width, stdio_flags_t flags)
{
	size_t unpadded_len = len;

	if(!(flags & FlagLeft))
	{
		if(width && (flags & FlagZeropad) && (negative || (flags & (FlagPlus | FlagSpace))))
		{
			width--;
		}

		while((flags & FlagZeropad) && (len < width) && (len < STDIO_INTEGER_BUFFER_SIZE))
		{
			buffer[len++] = '0';
		}
	}

	while((len < precision) && (len < STDIO_INTEGER_BUFFER_SIZE))
	{
		buffer[len++] = '0';
	}

	if((base == BaseOctal) && (len > unpadded_len))
	{
		flags &= ~FlagHash;
	}

	if(flags & (FlagHash | FlagPointer))
	{
		if(!(flags & FlagPrecision) && len && ((len == precision) || (len == width)))
		{
			if(unpadded_len < len)
			{
				len--;
			}

			if(len && ((base == BaseHex) || (base == BaseBinary)) && (unpadded_len < len))
			{
				len--;
			}
		}

		if((base == BaseHex) && !(flags & FlagUppercase) && (len < STDIO_INTEGER_BUFFER_SIZE))
		{
			buffer[len++] = 'x';
		}
		else if((base == BaseHex) && (flags & FlagUppercase) && (len < STDIO_INTEGER_BUFFER_SIZE))
		{
			buffer[len++] = 'X';
		}
		else if((base == BaseBinary) && (len < STDIO_INTEGER_BUFFER_SIZE))
		{
			buffer[len++] = 'b';
		}

		if(len < STDIO_INTEGER_BUFFER_SIZE)
		{
			buffer[len++] = '0';
		}
	}

	if(len < STDIO_INTEGER_BUFFER_SIZE)
	{
		if(negative)
		{
			buffer[len++] = '-';
		}
		else if(flags & FlagPlus)
		{
			buffer[len++] = '+';
		}
		else if(flags & FlagSpace)
		{
			buffer[len++] = ' ';
		}
	}

	StdioOutputReverse(fp, buffer, len, width, flags);
}

void StdioPrintInt(FILE* fp, size_t value, bool negative, uint8_t base, size_t precision,
				   size_t width, stdio_flags_t flags)
{
	char buffer[STDIO_INTEGER_BUFFER_SIZE];
	size_t len = 0;

	if(!value)
	{
		if(!(flags & FlagPrecision))
		{
			buffer[len++] = '0';
			flags &= ~FlagHash;
		}
		else if(base == BaseHex)
		{
			flags &= ~FlagHash;
		}
	}
	else
	{
		do
		{
			const char digit = static_cast<char>(value % base);
			buffer[len++] = static_cast<char>(
				digit < 10 ? '0' + digit : (flags & FlagUppercase ? 'A' : 'a') + digit - 10);
			value /= base;
		} while(value && (len < STDIO_INTEGER_BUFFER_SIZE));
	}

	StdioPrintInteger(fp, buffer, len, negative, base, precision, width, flags);
}

void StdioFormatString(FILE* fp, const char* format, va_list args)
{
#define ADVANCE_IN_FORMAT_STRING(ptr) \
	do                                \
	{                                 \
		(ptr)++;                      \
		if(!*(ptr))                   \
		{                             \
			return;                   \
		}                             \
	} while(0)

	while(*format)
	{
		if(*format != '%')
		{
			fputc(*format, fp);
			format++;
			continue;
		}

		ADVANCE_IN_FORMAT_STRING(format);

		stdio_flags_t flags = StdioParseFlags(&format);
		size_t width = 0;

		if(isdigit(*format))
		{
			width = static_cast<size_t>(StdioAtou(&format));
		}
		else if(*format == '*')
		{
			const int val = va_arg(args, int);

			if(val < 0)
			{
				flags |= FlagLeft;
				width = static_cast<size_t>(-val);
			}
			else
			{
				width = static_cast<size_t>(val);
			}

			ADVANCE_IN_FORMAT_STRING(format);
		}

		size_t precision = 0;

		if(*format == '.')
		{
			flags |= FlagPrecision;
			ADVANCE_IN_FORMAT_STRING(format);

			if(*format == '-')
			{
				do
				{
					ADVANCE_IN_FORMAT_STRING(format);
				} while(isdigit(*format));

				flags &= ~FlagPrecision;
			}
			else if(isdigit(*format))
			{
				precision = StdioAtou(&format);
			}
			else if(*format == '*')
			{
				const int precision_ = va_arg(args, int);

				if(precision_ < 0)
				{
					flags &= ~FlagPrecision;
				}
				else
				{
					precision = (precision_ > 0) ? static_cast<size_t>(precision_) : 0;
				}

				ADVANCE_IN_FORMAT_STRING(format);
			}
		}

		switch(*format)
		{
			case 'I':
			{
				ADVANCE_IN_FORMAT_STRING(format);

				switch(*format)
				{
					case '8':
						flags |= FlagInt8;
						ADVANCE_IN_FORMAT_STRING(format);
						break;
					case '1':
						ADVANCE_IN_FORMAT_STRING(format);
						if(*format == '6')
						{
							format++;
							flags |= FlagInt16;
						}
						break;
					case '3':
						ADVANCE_IN_FORMAT_STRING(format);
						if(*format == '2')
						{
							ADVANCE_IN_FORMAT_STRING(format);
							flags |= FlagInt32;
						}
						break;
					case '6':
						ADVANCE_IN_FORMAT_STRING(format);
						if(*format == '4')
						{
							ADVANCE_IN_FORMAT_STRING(format);
							flags |= FlagInt64;
						}
						break;
					default:
						break;
				}
				break;
			}

			case 'l':
				flags |= FlagLong;
				ADVANCE_IN_FORMAT_STRING(format);
				if(*format == 'l')
				{
					flags |= FlagLongLong;
					ADVANCE_IN_FORMAT_STRING(format);
				}
				break;
			case 'h':
				flags |= FlagShort;
				ADVANCE_IN_FORMAT_STRING(format);
				if(*format == 'h')
				{
					flags |= FlagChar;
					ADVANCE_IN_FORMAT_STRING(format);
				}
				break;
			case 't':
				flags |= (sizeof(ptrdiff_t) <= sizeof(int))	 ? FlagInt :
						 (sizeof(ptrdiff_t) == sizeof(long)) ? FlagLong :
															   FlagLongLong;
				ADVANCE_IN_FORMAT_STRING(format);
				break;
			case 'j':
				flags |= (sizeof(intmax_t) == sizeof(long) ? FlagLong : FlagLongLong);
				ADVANCE_IN_FORMAT_STRING(format);
				break;
			case 'z':
				flags |= (sizeof(size_t) <= sizeof(int))  ? FlagInt :
						 (sizeof(size_t) == sizeof(long)) ? FlagLong :
															FlagLongLong;
				ADVANCE_IN_FORMAT_STRING(format);
				break;
			default:
				break;
		}

		switch(*format)
		{
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
			case 'b':
			{
				if(*format == 'd' || *format == 'i')
				{
					flags |= FlagSigned;
				}

				uint8_t base;
				if(*format == 'x' || *format == 'X')
				{
					base = BaseHex;
				}
				else if(*format == 'o')
				{
					base = BaseOctal;
				}
				else if(*format == 'b')
				{
					base = BaseBinary;
				}
				else
				{
					base = BaseDecimal;
					flags &= ~FlagHash;
				}

				if(*format == 'X')
				{
					flags |= FlagUppercase;
				}

				format++;
				if(flags & FlagPrecision)
				{
					flags &= ~FlagZeropad;
				}

				if(flags & FlagSigned)
				{
					if(flags & FlagLongLong)
					{
						const long long value = va_arg(args, long long);
						StdioPrintInt(fp, STDIO_ABS(value), value < 0, base, precision, width,
									  flags);
					}
					else if(flags & FlagLong)
					{
						const long value = va_arg(args, long);
						StdioPrintInt(fp, STDIO_ABS(value), value < 0, base, precision, width,
									  flags);
					}
					else
					{
						const int value = (flags & FlagChar)  ? (signed char)va_arg(args, int) :
										  (flags & FlagShort) ? (short int)va_arg(args, int) :
																va_arg(args, int);
						StdioPrintInt(fp, STDIO_ABS(value), value < 0, base, precision, width,
									  flags);
					}
				}
				else
				{
					flags &= ~(FlagPlus | FlagSpace);

					if(flags & FlagLongLong)
					{
						StdioPrintInt(fp, static_cast<size_t>(va_arg(args, unsigned long long)),
									  false, base, precision, width, flags);
					}
					else if(flags & FlagLong)
					{
						StdioPrintInt(fp, static_cast<size_t>(va_arg(args, unsigned long)), false,
									  base, precision, width, flags);
					}
					else
					{
						const unsigned int value =
							(flags & FlagChar) ?
								static_cast<unsigned char>(va_arg(args, unsigned int)) :
							(flags & FlagShort) ?
								static_cast<unsigned short int>(va_arg(args, unsigned int)) :
								va_arg(args, unsigned int);
						StdioPrintInt(fp, static_cast<size_t>(value), false, base, precision, width,
									  flags);
					}
				}
				break;
			}
			case 'c':
			{
				size_t l = 1U;

				if(!(flags & FlagLeft))
				{
					while(l++ < width)
					{
						fputc(' ', fp);
					}
				}

				fputc(static_cast<char>(va_arg(args, int)), fp);

				if(flags & FlagLeft)
				{
					while(l++ < width)
					{
						fputc(' ', fp);
					}
				}
				format++;
				break;
			}

			case 's':
			{
				const char* p = va_arg(args, char*);
				if(p == NULL)
				{
					StdioOutputReverse(fp, ")llun(", 6, width, flags);
				}
				else
				{
					size_t l = strnlen(p, precision ? precision : BUFSIZE);

					if(flags & FlagPrecision)
					{
						l = (l < precision ? l : precision);
					}
					if(!(flags & FlagLeft))
					{
						while(l++ < width)
						{
							fputc(' ', fp);
						}
					}

					while((*p != 0) && (!(flags & FlagPrecision) || precision))
					{
						fputc(*(p++), fp);
						--precision;
					}

					if(flags & FlagLeft)
					{
						while(l++ < width)
						{
							fputc(' ', fp);
						}
					}
				}
				format++;
				break;
			}

			case 'p':
			{
				width = sizeof(void*) * 2U + 2;
				flags |= FlagZeropad | FlagPointer;
				uintptr_t value = reinterpret_cast<uintptr_t>(va_arg(args, void*));
				(value == reinterpret_cast<uintptr_t>(nullptr)) ?
					StdioOutputReverse(fp, ")lin(", 5, width, flags) :
					StdioPrintInt(fp, static_cast<size_t>(value), false, BaseHex, precision, width,
								  flags);
				format++;
				break;
			}

			case '%':
				fputc('%', fp);
				format++;
				break;

			default:
				fputc(*format, fp);
				format++;
				break;
		}
	}
}
} // namespace details

int StdioVsnprintf(FILE* fp, const char* format, va_list args)
{
	details::StdioFormatString(fp, format, args);
	details::StdioAppendTermination(fp);

	return static_cast<int>(fp->position);
}