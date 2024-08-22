#include "memory/memory.hpp"
#include "mmu.hpp"
#include <memory/vmm.hpp>
#include <utility>

namespace artemis
{
namespace memory
{
namespace details
{
inline std::size_t GetPageSize(std::size_t flags)
{
	if(flags & MAP_PAGE_1GB)
	{
		return page_size_1GiB;
	}

	if(flags & MAP_PAGE_2MB)
	{
		return page_size_2MiB;
	}

	return page_size_4KiB;
}

std::size_t ParseCache(std::size_t flags, std::size_t page_size)
{
	std::size_t patbit = (page_size > page_size_4KiB) ? (1 << 12) : (1 << 7);
	std::size_t ret = 0;

	if(flags & MAP_MMIO)
	{
		ret |= PageFlagNoCache;
	}
	else if(flags & MAP_WRITE_COMBINING)
	{
		ret |= PageFlagNoCache | PageFlagWriteThrough;
	}
	else if(flags & MAP_WRITE_THROUGH)
	{
		ret |= patbit;
	}
	else if(flags & MAP_PROTECTED)
	{
		ret |= patbit | PageFlagWriteThrough;
	}
	else if(flags & MAP_WRITE_BACK)
	{
		ret |= patbit | PageFlagNoCache;
	}
	else if(flags & MAP_NO_CACHE)
	{
		ret |= patbit | PageFlagNoCache | PageFlagWriteThrough;
	}

	return ret;
}
} // namespace details

std::pair<std::size_t, std::size_t> VirtualMemoryManager::ParseFlags(std::size_t flags)
{
	std::size_t page_size = details::GetPageSize(flags);
	std::size_t arch_flags = details::ParseCache(flags, page_size);

	if((flags & MAP_PAGE_1GB) || (flags & MAP_PAGE_2MB))
	{
		arch_flags |= PageFlagHugePage;
	}

	if(flags & MAP_READ)
	{
		arch_flags |= PageFlagPresent;
	}

	if(flags & MAP_WRITE)
	{
		arch_flags |= PageFlagWritable;
	}

	if(flags & MAP_USER)
	{
		arch_flags |= PageFlagUserAccessible;
	}

	if(!(flags & MAP_EXEC))
	{
		arch_flags |= PageFlagNoExecute;
	}

	if(flags & MAP_GLOBAL)
	{
		arch_flags |= PageFlagGlobal;
	}

	return std::make_pair(arch_flags, page_size);
}
} // namespace memory
} // namespace artemis