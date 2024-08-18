#ifndef AMD64_CPU_REGISTERS_HPP
#define AMD64_CPU_REGISTERS_HPP

#include <cstdint>

namespace artemis
{
// https://www.amd.com/content/dam/amd/en/documents/processor-tech-docs/programmer-references/24593.pdf
enum PatEncodings : std::size_t
{
	PatForceUncachable = 0,
	PatWriteCombining = 1,
	PatWriteThrough = 4,
	PatWriteProtect = 5,
	PatWriteBack = 6,
	PatUncachable = 7,
};

inline std::uint64_t ReadCr0()
{
	std::uint64_t value = 0;
	asm volatile("mov %%cr0, %0" : "=r"(value)::"memory");
	return value;
}

inline std::uint64_t ReadCr2()
{
	std::uint64_t value = 0;
	asm volatile("mov %%cr2, %0" : "=r"(value)::"memory");
	return value;
}

inline std::uint64_t ReadCr3()
{
	std::uint64_t value = 0;
	asm volatile("mov %%cr3, %0" : "=r"(value)::"memory");
	return value;
}

inline std::uint64_t ReadCr4()
{
	std::uint64_t value = 0;
	asm volatile("mov %%cr4, %0" : "=r"(value)::"memory");
	return value;
}

inline void WriteCr0(std::uint64_t value)
{
	asm volatile("mov %0, %%cr0" ::"r"(value) : "memory");
}

inline void WriteCr2(std::uint64_t value)
{
	asm volatile("mov %0, %%cr2" ::"r"(value) : "memory");
}

inline void WriteCr3(std::uint64_t value)
{
	asm volatile("mov %0, %%cr3" ::"r"(value) : "memory");
}

inline void WriteCr4(std::uint64_t value)
{
	asm volatile("mov %0, %%cr4" ::"r"(value) : "memory");
}

inline void InvalidatePage(std::uintptr_t address)
{
	asm volatile("invlpg (%0)" ::"r"(address));
}

inline std::uint64_t ReadMsr(std::uint32_t msr)
{
	std::uint32_t edx, eax;
	asm volatile("rdmsr" : "=a"(eax), "=d"(edx) : "c"(msr) : "memory");
	return (static_cast<std::uint64_t>(edx) << 32) | eax;
}

inline void WriteMsr(std::uint32_t msr, std::uint64_t value)
{
	std::uint32_t edx = value >> 32;
	std::uint32_t eax = static_cast<std::uint32_t>(value);
	asm volatile("wrmsr" ::"a"(eax), "d"(edx), "c"(msr) : "memory");
}

void EnablePat();
} // namespace artemis

#endif // AMD64_CPU_REGISTERS_HPP