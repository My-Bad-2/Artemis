#include "arch.hpp"
#include <cpu/cpu.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>

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
} // namespace cpu
} // namespace artemis