#include <cpu/cpuid.hpp>

namespace artemis
{
namespace cpu
{
CpuidLeaf ReadCpuid(std::uint32_t leaf_num, std::uint32_t subleaf_num)
{
	static std::uint32_t cpuid_max = 0;
	CpuidLeaf leaf;

	if(cpuid_max == 0)
	{
		asm volatile("cpuid" : "=a"(cpuid_max) : "a"(leaf_num & 0x80000000) : "ebx", "ecx", "edx");
	}

	if(leaf_num > cpuid_max)
	{
		return leaf;
	}

	asm volatile("cpuid"
				 : "=a"(leaf.values[0]), "=b"(leaf.values[1]), "=c"(leaf.values[2]),
				   "=d"(leaf.values[3])
				 : "a"(leaf_num), "c"(subleaf_num));

	return leaf;
}
} // namespace cpu
} // namespace artemis