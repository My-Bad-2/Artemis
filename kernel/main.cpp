#include <limine.h>
#include <arch.hpp>

extern "C" void kernel_main()
{
	using namespace artemis;

	arch::Initialize();
	arch::Halt(false);
}