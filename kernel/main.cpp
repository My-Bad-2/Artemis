#include <stdio.h>
#include <arch.hpp>

extern "C" void kernel_main()
{
	using namespace artemis;

	arch::Initialize();

	printf("Hello, World!\n");

	arch::Halt(false);
}