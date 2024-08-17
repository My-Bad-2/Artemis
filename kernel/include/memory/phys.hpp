#ifndef MEMORY_PHYS_HPP
#define MEMORY_PHYS_HPP

#include <libs/bitmap.hpp>
#include <cstdint>

namespace artemis
{
namespace memory
{
struct PhysicalStats
{
	std::size_t total_memory;
	std::size_t free_memory;
	std::size_t used_memory;
	std::size_t usable_memory;
};

class PhysicalMemoryManager
{
  public:
	PhysicalMemoryManager() = default;

	void Initialize();

	void* Allocate(std::size_t page_count = 1);
	void Free(void* ptr, std::size_t page_count = 1);

	PhysicalStats& GetStats();

  private:
	void* Alloc(std::size_t page_count, std::size_t limit);

	Bitmap<std::uint32_t> bitmap_;
	PhysicalStats statistics_;
	std::size_t last_index_;
    std::uintptr_t highest_usable_addr_;
};
} // namespace memory
} // namespace artemis

#endif // MEMORY_PHYS_HPP