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

__CDECLS_BEGIN

void ExceptionHandler(void* frame)
{
	Iframe* iframe = static_cast<Iframe*>(frame);

	if(!global_idt.HandleInterrupts(iframe))
	{
		LogDebug("Interrupt Handler %lu not registered", iframe->vector);
	}
}

void NmiHandler(void* frame)
{
	LogPanic("NMI Handler called!");
}

__CDECLS_END
} // namespace cpu
} // namespace artemis