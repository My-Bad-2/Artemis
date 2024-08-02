#include <arch.hpp>
#include <devices/serials.hpp>

namespace artemis
{
namespace arch
{
devices::SerialDevice serials = devices::SerialDevice(ComPort1);

void Initialize()
{
	serials.Initialize();
	serials.Write("Hello, World!\n");
}
} // namespace arch
} // namespace artemis