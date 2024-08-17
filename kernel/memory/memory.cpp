#include <memory/phys.hpp>
#include <memory/memory.hpp>
#include <logger.hpp>

namespace artemis
{
namespace memory
{
PhysicalMemoryManager physical_memory_manager;

void Initialize()
{
	physical_memory_manager.Initialize();
}
} // namespace memory
} // namespace artemis