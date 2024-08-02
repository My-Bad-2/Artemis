#include <stdio.h>

int putchar(int ch) {
    return fputc(ch, stdout);
}

int puts(const char* restrict str) {
    return fputs(str, stdout);
}