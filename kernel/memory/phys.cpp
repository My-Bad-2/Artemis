#include <algorithm>
#include <memory/memory.hpp>
#include <memory/phys.hpp>
#include <string.h>
#include <logger.hpp>

namespace artemis
{
namespace memory
{
void* PhysicalMemoryManager::Alloc(std::size_t page_count, std::size_t limit)
{
	std::size_t page = 0;

	while(this->last_index_ < limit)
	{
		if(this->bitmap_[last_index_++] == BitmapAvailable)
		{
			if(++page == page_count)
			{
				std::size_t page_index = last_index_ - page_count;

				for(std::size_t i = page_index; i < this->last_index_; i++)
				{
					bitmap_.Set(i, BitmapUsed);
				}

				return reinterpret_cast<void*>(page_index * page_size_4KiB);
			}
		}
		else
		{
			page = 0;
		}
	}

	return nullptr;
}

void* PhysicalMemoryManager::Allocate(std::size_t page_count)
{
	if(page_count == 0)
	{
		return nullptr;
	}

	void* ret = this->Alloc(page_count, this->highest_usable_addr_ / page_size_4KiB);

	if(ret == nullptr)
	{
		std::size_t index = this->last_index_;
		this->last_index_ = 0;
		ret = this->Alloc(page_count, index);

		if(ret == nullptr)
		{
			LogFatal("Out of Physical Memory!");
		}
	}

	memset(ToHigherHalf(ret), 0, page_count * page_size_4KiB);

	this->statistics_.used_memory += page_count * page_size_4KiB;
	this->statistics_.free_memory += page_count * page_size_4KiB;

	return ret;
}

void PhysicalMemoryManager::Free(void* ptr, std::size_t count)
{
	if(ptr == nullptr)
	{
		return;
	}

	std::size_t page = reinterpret_cast<std::size_t>(ptr) / page_size_4KiB;

	for(std::size_t i = page; i < (page + count); i++)
	{
		this->bitmap_.Set(i, BitmapAvailable);
	}

	this->statistics_.used_memory -= count * page_size_4KiB;
	this->statistics_.free_memory += count * page_size_4KiB;
}

PhysicalStats& PhysicalMemoryManager::GetStats()
{
	return this->statistics_;
}

void PhysicalMemoryManager::Initialize()
{
	limine_memmap_entry** memmaps = memmap_request.response->entries;
	const std::size_t memmap_count = memmap_request.response->entry_count;

	std::uintptr_t highest_addr = 0;

	for(size_t i = 0; i < memmap_count; i++)
	{
		if(memmaps[i]->type != LIMINE_MEMMAP_USABLE)
		{
			continue;
		}

		std::uintptr_t top = memmaps[i]->base + memmaps[i]->length;
		highest_addr = std::max(highest_addr, top);

		this->statistics_.usable_memory += memmaps[i]->length;
		this->statistics_.total_memory += memmaps[i]->length;

		this->highest_usable_addr_ = std::max(this->highest_usable_addr_, top);
	}

	std::size_t bitmap_entries = this->highest_usable_addr_ / page_size_4KiB;
	std::size_t bitmap_size = AlignUp(bitmap_entries / 32, page_size_4KiB);
	bitmap_entries = bitmap_size * 32;

	for(std::size_t i = 0; i < memmap_count; i++)
	{
		if(memmaps[i]->type != LIMINE_MEMMAP_USABLE)
		{
			continue;
		}

		if(memmaps[i]->length >= bitmap_size)
		{
			std::uint32_t* bitmap_base =
				reinterpret_cast<std::uint32_t*>(ToHigherHalf(memmaps[i]->base));
			this->bitmap_.Initialize(bitmap_base, bitmap_entries);
			this->bitmap_.Fill(BitmapUsed);

			memmaps[i]->base += bitmap_size;
			memmaps[i]->length -= bitmap_size;

			this->statistics_.used_memory += bitmap_size;
#ifdef DEBUG
			LogDebug("Initialized Bitmap at %p for total entries: %lu", bitmap_base,
					 bitmap_entries);
#endif // DEBUG
			break;
		}
	}

	for(std::size_t i = 0; i < memmap_count; i++)
	{
		if(memmaps[i]->type != LIMINE_MEMMAP_USABLE)
		{
			continue;
		}

		for(std::uintptr_t j = 0; j < memmaps[i]->length; j += page_size_4KiB)
		{
			this->bitmap_.Set((memmaps[i]->base + j) / page_size_4KiB, BitmapAvailable);
		}
	}

	this->statistics_.free_memory = this->statistics_.usable_memory - this->statistics_.used_memory;

#ifdef DEBUG
	LogDebug("Initialized Physical Memory Manager. Total Memory(in MB): %lu",
			 this->statistics_.total_memory / 1024 / 1024);
#endif // DEBUG
}
} // namespace memory
} // namespace artemis