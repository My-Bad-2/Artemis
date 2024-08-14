#ifndef LIBS_STACKTRACE_HPP
#define LIBS_STACKTRACE_HPP

#include <cstdint>

namespace artemis
{
struct StackFrame
{
	StackFrame* next;
	std::uintptr_t return_address;
};

void DumpStackTrace();
} // namespace artemis

#endif // LIBS_STACKTRACE_HPP