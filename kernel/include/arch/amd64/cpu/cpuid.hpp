#ifndef AMD64_CPU_CPUID_HPP
#define AMD64_CPU_CPUID_HPP

#include <array>
#include <cstdint>
#include <utility>

namespace artemis
{
namespace cpu
{
enum CpuidRegister : std::uint8_t
{
	kEax = 0,
	kEbx = 1,
	kEcx = 2,
	kEdx = 3,
};

struct CpuidLeaf
{
	std::uint32_t Read(CpuidRegister reg) const
	{
		return this->values[std::to_underlying(reg)];
	}

	std::array<std::uint32_t, 4> values;
};

CpuidLeaf ReadCpuid(std::uint32_t leaf_num, std::uint32_t subleaf_num);
} // namespace cpu
} // namespace artemis

#endif // AMD64_CPU_CPUID_HPP