#include <arch.hpp>
#include <cpu/cpu.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <logger.hpp>

namespace artemis
{
namespace cpu
{
Gdt global_gdt = {};
Idt global_idt = {};

void Initialize()
{
	arch::DisableInterrupts();

	global_gdt.Initialize();
	global_idt.Initialize();

	arch::EnableInterrupts();
}

extern "C"
{
	void ExceptionHandler(void* rdi)
	{
		Iframe* iframe = static_cast<Iframe*>(rdi);

		if(!global_idt.HandleInterrupts(iframe))
		{
			LogDebug("Interrupt Handler %lu not registered", iframe->vector);
		}
	}

	void NmiHandler()
	{
		LogPanic("NMI Handler called!");
	}
}
} // namespace cpu
} // namespace artemis