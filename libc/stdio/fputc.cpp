#include <stdio.h>
#include <string.h>

int fputc(int ch, FILE* fp)
{
	size_t write_pos = fp->position++;

	if(write_pos >= fp->max_chars)
	{
		return 0;
	}

	if(fp->write != NULL)
	{
		fp->write(ch, fp->args);
	}
	else
	{
		fp->buffer[write_pos] = ch;
	}

	return 1;
}

int fputs(const char* restrict str, FILE* fp)
{
	size_t len = strlen(str);

	for(size_t i = 0; i < len; i++)
	{
		if(!fputc(str[i], fp))
		{
			return 0;
		}
	}

	return len;
}