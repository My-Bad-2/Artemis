#include <logger.hpp>
#include <mmu.hpp>
#include <assert.h>

#include <cpu/features.hpp>
#include <cpu/registers.hpp>
#include <memory/paging.hpp>

#define GET_PML_ENTRY(virtual_address, offset) ((virtual_address & (0x1ffUL << offset)) >> offset)

namespace artemis
{
namespace memory
{
std::uintptr_t page_address_mask = 0x000FFFFFFFFFF000;
constexpr std::size_t default_page_flags =
	PageFlagPresent | PageFlagWritable | PageFlagUserAccessible;

bool PageEntry::IsValid()
{
	return this->GetFlags(PageFlagPresent);
}

bool PageEntry::IsLarge()
{
	return this->GetFlags(PageFlagHugePage);
}

std::size_t PageMap::PageSizeFlag(std::size_t page_size)
{
	if(page_size == page_size_4KiB)
	{
		return 0;
	}

	return PageFlagHugePage;
}

std::size_t PageMap::GetPageSize(std::size_t size)
{
	if(size >= page_size_1GiB)
	{
		return page_size_1GiB;
	}
	else if(size >= page_size_2MiB)
	{
		return page_size_2MiB;
	}

	return page_size_4KiB;
}

void* PageMap::GetNextLvl(PageEntry& entry, bool allocate, std::uintptr_t virtual_addr,
						  std::size_t old_page_size, std::size_t page_size)
{
	void* ret = nullptr;

	if(entry.IsValid())
	{
		if(entry.IsLarge() && old_page_size != -1)
		{
			std::size_t old_flags = entry.GetFlags();
			std::uintptr_t old_physical_address = entry.GetAddress();
			std::uintptr_t old_virtual_address = virtual_addr & ~(old_page_size - 1);

			if(old_physical_address & (old_page_size - 1))
			{
				LogPanic("Unexpected Page Table Entry Address: 0x%lx", old_physical_address);
			}

			ret = ToHigherHalf(this->physical_manager_->Allocate());
			entry.SetAddress(FromHigherHalf(reinterpret_cast<std::uintptr_t>(ret)));
			entry.SetFlag(default_page_flags);

			for(std::size_t i = 0; i < old_page_size; i += page_size)
			{
				this->Map(old_virtual_address + i, old_physical_address + i,
						  old_flags | this->PageSizeFlag(page_size), page_size);
			}
		}
		else
		{
			ret = ToHigherHalf(reinterpret_cast<void*>(entry.GetAddress()));
		}
	}
	else if(allocate)
	{
		ret = ToHigherHalf(physical_manager_->Allocate());
		entry.SetAddress(FromHigherHalf(reinterpret_cast<std::uintptr_t>(ret)));
		entry.SetFlag(default_page_flags);
	}

	return ret;
}

PageEntry* PageMap::VirtualToPageEntry(std::uintptr_t virtual_address, bool allocate,
									   std::size_t page_size, bool check_large)
{
	const std::size_t pml5_entry = GET_PML_ENTRY(virtual_address, 48);
	const std::size_t pml4_entry = GET_PML_ENTRY(virtual_address, 39);
	const std::size_t pdp_entry = GET_PML_ENTRY(virtual_address, 30);
	const std::size_t pd_entry = GET_PML_ENTRY(virtual_address, 21);
	const std::size_t pt_entry = GET_PML_ENTRY(virtual_address, 12);

	if(this->top_lvl_ == nullptr)
	{
		return nullptr;
	}

	PageTable* pml4 = this->top_lvl_;

	if(IsPagingModeMax())
	{
		pml4 = static_cast<PageTable*>(
			this->GetNextLvl(this->top_lvl_->entries[pml5_entry], allocate));
	}

	if(pml4 == nullptr)
	{
		return nullptr;
	}

	PageTable* pdp = static_cast<PageTable*>(this->GetNextLvl(pml4->entries[pml4_entry], allocate));

	if(pdp == nullptr)
	{
		return nullptr;
	}

	if((page_size == page_size_1GiB) || (check_large && pdp->entries[pdp_entry].IsLarge()))
	{
		return &pdp->entries[pdp_entry];
	}

	PageTable* pd = static_cast<PageTable*>(this->GetNextLvl(
		pdp->entries[pdp_entry], allocate, virtual_address, page_size_1GiB, page_size));

	if(pd == nullptr)
	{
		return nullptr;
	}

	if((page_size == page_size_2MiB) || (check_large && pd->entries[pd_entry].IsLarge()))
	{
		return &pd->entries[pd_entry];
	}

	PageTable* pt = static_cast<PageTable*>(this->GetNextLvl(
		pd->entries[pd_entry], allocate, virtual_address, page_size_2MiB, page_size));

	if(pt == nullptr)
	{
		return nullptr;
	}

	return &pt->entries[pt_entry];
}

std::uintptr_t PageMap::VirtualToPhysical(std::uintptr_t virtual_address, std::size_t page_size)
{
	PageEntry* page_entry = this->VirtualToPageEntry(virtual_address, false, page_size, true);

	if((page_entry == nullptr) || !page_entry->GetFlags(PageFlagPresent))
	{
		return invalid_address;
	}

	return page_entry->GetAddress() + (virtual_address % page_size);
}

bool PageMap::Map(std::uintptr_t virtual_address, std::uintptr_t physical_address,
				  std::size_t flags, std::size_t page_size)
{
	auto map_page = [this](std::uintptr_t virtual_address, std::uintptr_t physical_address,
						   std::size_t flags, std::size_t page_size) {
		PageEntry* page_entry = this->VirtualToPageEntry(virtual_address, true, page_size, false);

		if(page_entry == nullptr)
		{
			LogError("Could not get Page map Entry for address 0x%lx", virtual_address);
			return false;
		}

		page_entry->Reset();
		page_entry->SetAddress(physical_address);
		page_entry->SetFlag(flags);

		return true;
	};

	if((page_size == page_size_1GiB) && !this->pml3_translation_)
	{
		page_size = page_size_2MiB;

		for(std::size_t i = 0; i < page_size_1GiB; i += page_size_2MiB)
		{
			if(!map_page(virtual_address + i, physical_address + i, flags, page_size))
			{
				return false;
			}
		}

		return true;
	}

	return map_page(virtual_address, physical_address, flags, page_size);
}

bool PageMap::Unmap(std::uintptr_t virtual_address, std::size_t page_size)
{
	auto unmap_page = [this](std::uintptr_t virtual_address, std::size_t page_size) {
		PageEntry* page_entry = this->VirtualToPageEntry(virtual_address, false, page_size, true);

		if(page_entry == nullptr)
		{
			LogError("Could not get Page map Entry for address 0x%lx", virtual_address);
			return false;
		}

		page_entry->Reset();
		InvalidatePage(virtual_address);

		return true;
	};

	if((page_size == page_size_1GiB) && !this->pml3_translation_)
	{
		page_size = page_size_2MiB;

		for(std::size_t i = 0; i < page_size_1GiB; i += page_size_2MiB)
		{
			if(!unmap_page(virtual_address + i, page_size))
			{
				return false;
			}
		}

		return true;
	}

	return unmap_page(virtual_address, page_size);
}

bool PageMap::SetFlags(std::uintptr_t virtual_address, std::size_t flags, std::size_t page_size)
{
	PageEntry* page_entry = this->VirtualToPageEntry(virtual_address, false, page_size, true);

	if(page_entry == nullptr)
	{
		LogError("Could not get Page map Entry for address 0x%lx", virtual_address);
		return false;
	}

	std::uintptr_t physical_address = page_entry->GetAddress();

	page_entry->Reset();
	page_entry->SetAddress(physical_address);
	page_entry->SetFlag(flags);

	return true;
}

bool PageMap::Remap(std::uintptr_t new_virtual_address, std::uintptr_t virtual_address,
					std::size_t flags, std::size_t page_size)
{
	std::uintptr_t physical_address = this->VirtualToPhysical(virtual_address, page_size);
	this->Unmap(virtual_address, page_size);
	return this->Map(new_virtual_address, physical_address, flags, page_size);
}

bool PageMap::Map(std::uintptr_t virtual_address, std::uintptr_t physical_address,
				  std::size_t flags, std::size_t page_size, std::size_t size)
{
	for(std::size_t i = 0; i < size; i += page_size)
	{
		if(!this->Map(virtual_address + i, physical_address + i, flags, page_size))
		{
			this->Unmap(virtual_address, page_size, i - page_size);
			return true;
		}
	}

	return true;
}

bool PageMap::Unmap(std::uintptr_t virtual_address, std::size_t page_size, std::size_t size)
{
	for(std::size_t i = 0; i < size; i += page_size)
	{
		if(!this->Unmap(virtual_address + i, page_size))
		{
			return false;
		}
	}

	return true;
}

bool PageMap::SetFlags(std::uintptr_t virtual_address, std::size_t flags, std::size_t page_size,
					   std::size_t size)
{
	for(std::size_t i = 0; i < size; i += page_size)
	{
		if(!this->SetFlags(virtual_address + i, flags, page_size))
		{
			return false;
		}
	}

	return true;
}

bool PageMap::Remap(std::uintptr_t new_virtual_address, std::uintptr_t virtual_address,
					std::size_t flags, std::size_t page_size, std::size_t size)
{
	for(std::size_t i = 0; i < size; i += page_size)
	{
		if(!this->Remap(new_virtual_address + i, virtual_address + i, flags, page_size))
		{
			return false;
		}
	}

	return true;
}

void PageMap::Load()
{
	WriteCr3(FromHigherHalf(reinterpret_cast<std::uintptr_t>(this->top_lvl_)));
}

void PageMap::Save()
{
	this->top_lvl_ = reinterpret_cast<PageTable*>(ToHigherHalf(ReadCr3()));
}

void PageMap::DestroyLevel(PageTable* page_table, int start, int end, int level)
{
	if((level == 0) || (page_table == nullptr))
	{
		return;
	}

	for(int i = start; i < end; i++)
	{
		PageTable* next = static_cast<PageTable*>(this->GetNextLvl(page_table->entries[i], false));

		if(next == nullptr)
		{
			continue;
		}

		DestroyLevel(next, 0, 512, level - 1);
	}

	this->physical_manager_->Free(FromHigherHalf(page_table));
}

void PageMap::Initialize(PhysicalMemoryManager* physical_manager)
{
	this->physical_manager_ = physical_manager;
	this->pml3_translation_ = cpu::FeatureTest(FEATURE_HUGE_PAGE);

	this->top_lvl_ = static_cast<PageTable*>(ToHigherHalf(this->physical_manager_->Allocate()));

	for(std::size_t i = 256; i < pagetable_entries; i++)
	{
		this->GetNextLvl(this->top_lvl_->entries[i], true);
	}

	EnablePat();

	// Don't Map kernel for non-kernel pagemaps
	this->MapKernel();
}

void PageMap::MapKernel()
{
	limine_memmap_entry** memmaps = memmap_request.response->entries;
	const std::size_t memmap_count = memmap_request.response->entry_count;

	// Map Kernel's Lower 4 GiB
	for(std::uintptr_t i = 0; i < (page_size_1GiB * 4); i += page_size_1GiB)
	{
		assert(this->Map(ToHigherHalf(i), i,
						 PageFlagPresent | PageFlagWritable | PageFlagHugePage | PageFlagNoCache |
							 PageFlagNoExecute,
						 page_size_1GiB));
	}

	for(std::size_t i = 0; i < memmap_count; i++)
	{
		limine_memmap_entry* entry = memmaps[i];

		std::uintptr_t base = AlignDown(entry->base, page_size_4KiB);
		std::uintptr_t top = AlignUp(entry->base + entry->length, page_size_4KiB);

		// Don't map lower 4 GiB
		if(top < (page_size_1GiB * 4))
		{
			continue;
		}

		std::size_t cache = PageFlagNoCache;

		if(entry->type == LIMINE_MEMMAP_FRAMEBUFFER)
		{
			cache = PageFlagNoCache | PageFlagWriteThrough;
		}

		std::size_t size = top - base;
		std::size_t page_size = this->GetPageSize(size);
		std::size_t page_flag = this->PageSizeFlag(page_size);

		std::size_t aligned_size = AlignDown(size, page_size);
		std::size_t diff = size - aligned_size;

		for(std::uintptr_t j = base; j < (base + aligned_size); j += page_size)
		{
			if(top < (page_size_1GiB * 4))
			{
				continue;
			}

			assert(this->Map(ToHigherHalf(j), j,
							 PageFlagPresent | PageFlagWritable | page_flag | cache |
								 PageFlagNoExecute));
		}

		base += aligned_size;

		for(std::uintptr_t j = base; j < (base + diff); j += page_size_4KiB)
		{
			if(top < (page_size_1GiB * 4))
			{
				continue;
			}

			assert(this->Map(ToHigherHalf(j), j,
							 PageFlagPresent | PageFlagWritable | cache | PageFlagNoExecute));
		}
	}

	for(std::size_t i = 0; i < kernel_file_request.response->kernel_file->size; i += page_size_4KiB)
	{
		std::uintptr_t physical_address = kernel_address_request.response->physical_base + i;
		std::uintptr_t virtual_address = kernel_address_request.response->virtual_base + i;

		assert(this->Map(virtual_address, physical_address,
						 PageFlagPresent | PageFlagWritable | PageFlagNoCache));
	}

	this->Load();
}
} // namespace memory
} // namespace artemis