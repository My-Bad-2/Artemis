#ifndef MEMORY_VMM_HPP
#define MEMORY_VMM_HPP

#include <memory/phys.hpp>
#include <memory/paging.hpp>
#include <utility>

namespace artemis
{
enum VmmFlags : std::size_t
{
	MAP_READ = 0,
	MAP_WRITE = (1 << 0),
	MAP_EXEC = (1 << 2),
	MAP_GLOBAL = (1 << 3),
	MAP_USER = (1 << 4),
	MAP_PAGE_1GB = (1 << 5),
	MAP_PAGE_2MB = (1 << 6),
	MAP_MMIO = (1 << 7),
	MAP_WRITE_THROUGH = (1 << 6),
	MAP_PROTECTED = (1 << 7),
	MAP_WRITE_COMBINING = (1 << 8),
	MAP_WRITE_BACK = (1 << 9),
	MAP_NO_CACHE = (1 << 10),
};

namespace memory
{
class VirtualMemoryManager
{
  public:
	VirtualMemoryManager() = default;

	void Initialize(PhysicalMemoryManager* physical_memory_manager);

	bool Map(std::uintptr_t virtual_address, std::uintptr_t physical_address, std::size_t flags);
	bool Unmap(std::uintptr_t virtual_address, std::size_t flags);
	bool Remap(std::uintptr_t new_virtual_address, std::uintptr_t virtual_address,
			   std::size_t flags);
	bool Setflags(std::uintptr_t virtual_address, std::size_t flags);

	bool Map(std::uintptr_t virtual_address, std::uintptr_t physical_address, std::size_t flags,
			 std::size_t size);
	bool Unmap(std::uintptr_t virtual_address, std::size_t flags, std::size_t size);
	bool Remap(std::uintptr_t new_virtual_address, std::uintptr_t virtual_address,
			   std::size_t flags, std::size_t size);
	bool Setflags(std::uintptr_t virtual_address, std::size_t flags, std::size_t size);

	std::uintptr_t VirtualToPhysical(std::uintptr_t virtual_address, std::size_t flags);

	PageMap* get_pagemap()
	{
		return &this->active_pagemap_;
	}

  private:
	std::pair<std::size_t, std::size_t> ParseFlags(std::size_t flags);

	PhysicalMemoryManager* physical_memory_manager_;
	PageMap active_pagemap_;
};
} // namespace memory
} // namespace artemis

#endif // MEMORY_VMM_HPP