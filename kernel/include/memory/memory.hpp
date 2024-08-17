#ifndef MEMORY_MEMORY_HPP
#define MEMORY_MEMORY_HPP

#include <concepts>
#include <artemis.h>
#include <type_traits>

namespace artemis
{
namespace memory
{
constexpr std::size_t page_size_4KiB = 0x1000;
constexpr std::size_t page_size_2MiB = page_size_4KiB * 512;
constexpr std::size_t page_size_1GiB = page_size_2MiB * 512;

template<typename T>
using GetRet =
	std::conditional_t<std::integral<T>,
					   std::conditional_t<std::unsigned_integral<T>, uintptr_t, uintptr_t>, T>;

inline bool IsHigherHalf(auto addr)
{
	return uintptr_t(addr) >= HHDMOffset();
}

template<typename T, typename Ret = GetRet<T>>
inline constexpr Ret ToHigherHalf(T addr)
{
	return IsHigherHalf(addr) ? Ret(addr) : Ret(uintptr_t(addr) + HHDMOffset());
}

template<typename T, typename Ret = GetRet<T>>
inline constexpr Ret FromHigherHalf(T addr)
{
	return !IsHigherHalf(addr) ? Ret(addr) : Ret(uintptr_t(addr) - HHDMOffset());
}

template<std::integral T, std::integral U>
inline constexpr auto AlignDown(T addr, U align)
{
	return uint64_t(addr) & ~(align - 1);
}

template<std::integral T, std::integral U>
inline constexpr auto AlignUp(T addr, U align)
{
	return AlignDown(addr + align - 1, align);
}

template<std::integral T, std::integral U>
inline constexpr auto DivRoundUp(T addr, U align)
{
	return AlignDown(addr, align) / align;
}

void Initialize();
} // namespace memory
} // namespace artemis

#endif // MEMORY_MEMORY_HPP