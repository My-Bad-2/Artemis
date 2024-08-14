#include <libs/stacktrace.hpp>
#include <logger.hpp>
#include <sys/defs.h>

// Maximum number of stack traces
#define STACKTRACE_MAX 64

namespace artemis
{
void DumpStackTrace()
{
	LogTrace("Stacktrace: ");

	StackFrame* stackframe = reinterpret_cast<StackFrame*>(__GET_FRAME(0));

	int i = 0;

	while(stackframe != nullptr && i++ < STACKTRACE_MAX)
	{
		std::uintptr_t address = stackframe->return_address;

		if(address == 0)
		{
			break;
		}

		LogTrace("Frame #%d -> 0x%lx", i, address);

		stackframe = stackframe->next;
	}
}
} // namespace artemis