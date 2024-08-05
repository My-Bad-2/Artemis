#include <arch.hpp>
#include <devices/serials.hpp>

namespace artemis
{
namespace arch
{
devices::SerialDevice serials = devices::SerialDevice(ComPort1);

void Write(int ch)
{
	serials.Write(ch);
}

void Initialize()
{
	serials.Initialize();
}
} // namespace arch
} // namespace artemis