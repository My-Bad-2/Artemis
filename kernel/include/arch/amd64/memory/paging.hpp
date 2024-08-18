#ifndef AMD64_MEMORY_PAGING_HPP
#define AMD64_MEMORY_PAGING_HPP

#include <memory/memory.hpp>
#include <memory/phys.hpp>
#include <memory/virtual/page_entry.hpp>

namespace artemis
{
namespace memory
{
constexpr std::size_t pagetable_entries = 512;

struct PageTable
{
	PageEntry entries[pagetable_entries];
};

class PageMap
{
  public:
	PageMap() :
		top_lvl_(nullptr), physical_manager_(nullptr), pml3_translation_(false)
	{
	}

	~PageMap()
	{
		this->DestroyLevel(this->top_lvl_, 0, 256, IsPagingModeMax() ? 5 : 4);
	}

	void Initialize(PhysicalMemoryManager* physical_manager);

	bool Map(std::uintptr_t virtual_address, std::uintptr_t physical_address, std::size_t flags,
			 std::size_t page_size = page_size_4KiB);
	bool Unmap(std::uintptr_t virtual_address, std::size_t page_size);
	bool SetFlags(std::uintptr_t virtual_address, std::size_t flags,
				  std::size_t page_size = page_size_4KiB);
	bool Remap(std::uintptr_t new_virtual_address, std::uintptr_t virtual_address,
			   std::size_t flags, std::size_t page_size = page_size_4KiB);

	bool Map(std::uintptr_t virtual_address, std::uintptr_t physical_address, std::size_t flags,
			 std::size_t page_size, std::size_t size);
	bool Unmap(std::uintptr_t virtual_address, std::size_t page_size, std::size_t size);
	bool SetFlags(std::uintptr_t virtual_address, std::size_t flags, std::size_t page_size,
				  std::size_t size);
	bool Remap(std::uintptr_t new_virtual_address, std::uintptr_t virtual_address,
			   std::size_t flags, std::size_t page_size, std::size_t size);

	void Load();
	void Save();

  private:
	PageEntry* VirtualToPageEntry(std::uintptr_t virtual_address, bool allocate,
								  std::size_t page_size, bool check_large);
	std::uintptr_t VirtualToPhysical(std::uintptr_t virtual_address,
									 std::size_t page_size = page_size_4KiB);

	void* GetNextLvl(PageEntry& entry, bool allocate, std::uintptr_t virt_addr = -1,
					 std::size_t old_page_size = -1, std::size_t page_size = -1);

	std::size_t PageSizeFlag(std::size_t page_size);
	std::size_t GetPageSize(std::size_t size);

	void MapKernel();
	void DestroyLevel(PageTable* page_table, int start, int end, int level);

	PageTable* top_lvl_;
	PhysicalMemoryManager* physical_manager_;
	bool pml3_translation_;
};

PageMap* GetKernelPageMap();
} // namespace memory
} // namespace artemis

#endif // AMD64_MEMORY_PAGING_HPP