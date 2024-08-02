#ifndef AMD64_DEVICES_SERIALS_HPP
#define AMD64_DEVICES_SERIALS_HPP

#include <cstdint>
#include <string_view>
#include <utility>

enum serial_ports : std::uint16_t
{
	ComPort1 = 0x3f8,
	ComPort2 = 0x2f8,
	ComPort3 = 0x3e8,
	ComPort4 = 0x2e8,
};

namespace artemis
{
namespace devices
{
class SerialDevice
{
  public:
	constexpr SerialDevice() = default;
	constexpr SerialDevice(serial_ports port) : port_(std::to_underlying(port))
	{
	}

	SerialDevice(const SerialDevice&) = delete;
	SerialDevice(SerialDevice&&) = delete;

	SerialDevice& operator=(const SerialDevice&) = delete;
	SerialDevice& operator=(SerialDevice&&) = delete;

	bool Initialize();

	void Write(std::uint8_t ch);
	int Write(std::string_view string);

	inline std::uint16_t get_port() {
		return this->port_;
	}

  private:
	void WriteRegister(std::uint16_t reg, std::uint8_t val);
	std::uint8_t ReadRegister(std::uint16_t port);

	std::uint16_t port_;
};
} // namespace devices
} // namespace artemis

#endif // AMD64_DEVICES_SERIALS_HPP