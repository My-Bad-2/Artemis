#ifndef AMD64_ARCH_HPP
#define AMD64_ARCH_HPP

#include <concepts>
#include <cstdint>
#include <sys/defs.h>

namespace artemis
{
namespace arch
{
template<std::unsigned_integral T>
	requires(sizeof(T) <= sizeof(std::uint32_t))
inline T Inp(uint16_t port)
{
	T val = T(0);

	if constexpr(std::same_as<T, std::uint8_t>)
	{
		asm volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
	}
	else if constexpr(std::same_as<T, std::uint16_t>)
	{
		asm volatile("inw %1, %0" : "=a"(val) : "Nd"(port));
	}
	else if constexpr(std::same_as<T, std::uint32_t>)
	{
		asm volatile("inl %1, %0" : "=a"(val) : "Nd"(port));
	}

	return val;
}

template<std::unsigned_integral T>
	requires(sizeof(T) <= sizeof(std::uint32_t))
inline void Outp(uint16_t port, T val)
{
	if constexpr(std::same_as<T, std::uint8_t>)
	{
		asm volatile("outb %0, %1" ::"a"(val), "Nd"(port));
	}
	else if constexpr(std::same_as<T, std::uint16_t>)
	{
		asm volatile("outw %0, %1" ::"a"(val), "Nd"(port));
	}
	else
	{
		asm volatile("outl %0, %1" ::"a"(val), "Nd"(port));
	}
}

inline void Pause()
{
	asm volatile("pause");
}

inline void DisableInterrupts()
{
	asm volatile("cli");
}

inline void EnableInterrupts()
{
	asm volatile("sti");
}

__NO_RETURN inline void Halt(bool ints = true)
{
	if(ints)
	{
		while(true)
		{
			asm volatile("hlt");
		}
	}
	else
	{
		while(true)
		{
			asm volatile("cli");
			asm volatile("hlt");
		}
	}
}

void Initialize();
} // namespace arch
} // namespace artemis

#endif