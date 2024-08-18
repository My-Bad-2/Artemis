#include <memory/phys.hpp>
#include <memory/memory.hpp>
#include <logger.hpp>
#include <memory/paging.hpp>

namespace artemis
{
namespace memory
{
PhysicalMemoryManager physical_memory_manager;
PageMap kernel_pagemap;

PageMap* GetKernelPageMap() {
	return &kernel_pagemap;
}

void Initialize()
{
	physical_memory_manager.Initialize();
	kernel_pagemap.Initialize(&physical_memory_manager);
}
} // namespace memory
} // namespace artemis