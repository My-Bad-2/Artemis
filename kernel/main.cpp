#include <arch.hpp>
#include <logger.hpp>

extern "C" void kernel_main()
{
	using namespace artemis;

	arch::Initialize();

	LogDebug("Hello, World!");
	LogError("Hello, World!");
	LogWarning("Hello, World!");
	LogInfo("Hello, World!");
	LogTrace("Hello, World!");
	LogVerbose("Hello, World!");
	LogFatal("Hello, World!");

	arch::Halt(false);
}