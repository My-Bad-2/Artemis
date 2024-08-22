#include <memory/vmm.hpp>

namespace artemis
{
namespace memory
{
void VirtualMemoryManager::Initialize(PhysicalMemoryManager* physical_memory_manager)
{
	this->physical_memory_manager_ = physical_memory_manager;
	this->active_pagemap_.Initialize(physical_memory_manager);
}

bool VirtualMemoryManager::Map(std::uintptr_t virtual_address, std::uintptr_t physical_address,
							   std::size_t flags)
{
	auto [parsed_flags, page_size] = this->ParseFlags(flags);
	return this->active_pagemap_.Map(virtual_address, physical_address, flags, page_size);
}

bool VirtualMemoryManager::Unmap(std::uintptr_t virtual_address, std::size_t flags)
{
	auto [ignore, page_size] = this->ParseFlags(flags);
	static_cast<void>(ignore);
	
	return this->active_pagemap_.Unmap(virtual_address, page_size);
}

bool VirtualMemoryManager::Remap(std::uintptr_t new_virtual_address, std::uintptr_t virtual_address,
								 std::size_t flags)
{
	auto [parsed_flags, page_size] = this->ParseFlags(flags);
	return this->active_pagemap_.Remap(new_virtual_address, virtual_address, parsed_flags,
									   page_size);
}

bool VirtualMemoryManager::Setflags(std::uintptr_t virtual_address, std::size_t flags)
{
	auto [parsed_flags, page_size] = this->ParseFlags(flags);
	return this->active_pagemap_.SetFlags(virtual_address, parsed_flags, page_size);
}

bool VirtualMemoryManager::Map(std::uintptr_t virtual_address, std::uintptr_t physical_address,
							   std::size_t flags, std::size_t size)
{
	auto [parsed_flags, page_size] = this->ParseFlags(flags);
	return this->active_pagemap_.Map(virtual_address, physical_address, flags, page_size, size);
}

bool VirtualMemoryManager::Unmap(std::uintptr_t virtual_address, std::size_t flags,
								 std::size_t size)
{
	auto [ignore, page_size] = this->ParseFlags(flags);
	static_cast<void>(ignore);

	return this->active_pagemap_.Unmap(virtual_address, page_size, size);
}

bool VirtualMemoryManager::Remap(std::uintptr_t new_virtual_address, std::uintptr_t virtual_address,
								 std::size_t flags, std::size_t size)
{
	auto [parsed_flags, page_size] = this->ParseFlags(flags);
	return this->active_pagemap_.Remap(new_virtual_address, virtual_address, parsed_flags,
									   page_size, size);
}

bool VirtualMemoryManager::Setflags(std::uintptr_t virtual_address, std::size_t flags,
									std::size_t size)
{
	auto [parsed_flags, page_size] = this->ParseFlags(flags);
	return this->active_pagemap_.SetFlags(virtual_address, parsed_flags, page_size, size);
}

std::uintptr_t VirtualMemoryManager::VirtualToPhysical(std::uintptr_t virtual_address,
													   std::size_t flags)
{
	auto [ignore, page_size] = this->ParseFlags(flags);
	static_cast<void>(ignore);

	return this->active_pagemap_.VirtualToPhysical(virtual_address, page_size);
}
} // namespace memory
} // namespace artemis