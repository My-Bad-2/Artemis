#ifndef MEMORY_VIRTUAL_PAGE_ENTRY_HPP
#define MEMORY_VIRTUAL_PAGE_ENTRY_HPP

#include <cstddef>
#include <cstdint>

namespace artemis
{
namespace memory
{
extern std::uintptr_t page_address_mask;

struct PageEntry
{
	std::uintptr_t value = 0;

	void operator=(std::uintptr_t value)
	{
		this->value = value;
	}

	void Reset()
	{
		this->value = 0;
	}

	void SetFlag(std::size_t flags)
	{
		std::uintptr_t tmp = this->value;
		tmp &= ~flags;
		tmp |= flags;
		this->value = tmp;
	}

	bool GetFlags(std::size_t flags)
	{
		return (this->value & flags) ? true : false;
	}

	std::size_t GetFlags()
	{
		return this->value & ~page_address_mask;
	}

	std::uintptr_t GetAddress()
	{
		return this->value & page_address_mask;
	}

	void SetAddress(std::uintptr_t address)
	{
		std::uintptr_t tmp = this->value;
		tmp &= ~page_address_mask;
		tmp |= address;
		this->value = tmp;
	}

	bool IsValid();
	bool IsLarge();
};
} // namespace memory
} // namespace artemis

#endif // MEMORY_VIRTUAL_PAGE_ENTRY_HPP