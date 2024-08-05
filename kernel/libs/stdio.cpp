#include <arch.hpp>
#include <stdio.h>

#define STDIN (0)
#define STDOUT (1)
#define STDERR (2)

_Pragma("GCC diagnostic push");
_Pragma("GCC diagnostic ignored \"-Wc99-designator\"");

extern "C" FILE stdio_streams[3] = {
	[STDIN] = FILE(
		[](int ch, void* args) {
			artemis::arch::Write(ch);
		},
		nullptr),
	[STDOUT] = FILE(
		[](int ch, void* args) {
			artemis::arch::Write(ch);
		},
		nullptr),
	[STDERR] = FILE(
		[](int ch, void* args) {
			artemis::arch::Write(ch);
		},
		nullptr),
};

_Pragma("GCC diagnostic pop");