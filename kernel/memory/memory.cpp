#include <memory/phys.hpp>
#include <memory/memory.hpp>
#include <logger.hpp>
#include <memory/vmm.hpp>

namespace artemis
{
namespace memory
{
PhysicalMemoryManager physical_memory_manager;
VirtualMemoryManager virtual_memory_manager;

PageMap* GetKernelPageMap()
{
	return virtual_memory_manager.get_pagemap();
}

void Initialize()
{
	physical_memory_manager.Initialize();
	virtual_memory_manager.Initialize(&physical_memory_manager);
}
} // namespace memory
} // namespace artemis