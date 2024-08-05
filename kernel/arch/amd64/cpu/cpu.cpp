#include <cpu/cpu.hpp>
#include <cpu/gdt.hpp>

namespace artemis
{
namespace cpu
{
void Initialize()
{
	Gdt global_gdt;
	
	global_gdt.Initialize();
	global_gdt.Load();
}
} // namespace cpu
} // namespace artemis