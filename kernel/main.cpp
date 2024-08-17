#include <arch.hpp>
#include <logger.hpp>
#include <memory/memory.hpp>

extern "C" void kernel_main()
{
	using namespace artemis;

	arch::Initialize();
	memory::Initialize();

	LogDebug("Hello, World!");
	LogError("Hello, World!");
	LogWarning("Hello, World!");
	LogInfo("Hello, World!");
	LogTrace("Hello, World!");
	LogVerbose("Hello, World!");
	LogFatal("Hello, World!");

	arch::Halt(false);
}