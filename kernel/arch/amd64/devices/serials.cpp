#include <devices/serials.hpp>
#include <arch.hpp>

#define SERIALS_DATA 0
#define SERIALS_INTERRUPT 1
#define SERIALS_INTERRUPT_IDENTIFACTOR 2
#define SERIALS_LINE_CONTROL 3
#define SERIALS_MODEM_CONTROL 4
#define SERIALS_LINE_STATUS 5
#define SERIALS_MODEM_STATUS 6
#define SERIALS_SCRATCH 7

#define SERIALS_BAUD_RATE_LOW SERIALS_DATA
#define SERIALS_BAUD_RATE_HIGH SERIALS_INTERRUPT
#define SERIALS_FIFO_CONTROLLER SERIALS_INTERRUPT_IDENTIFACTOR

#define SERIALS_LINE_DS_5 0
#define SERIALS_LINE_DS_6 1
#define SERIALS_LINE_DS_7 2
#define SERIALS_LINE_DS_8 3
#define SERIALS_LINE_DLAB_STATUS (1 << 7)

#define SERIALS_MODEM_DTR (1 << 0)
#define SERIALS_MODEM_RTS (1 << 1)
#define SERIALS_MODEM_OUT1 (1 << 2)
#define SERIALS_MODEM_OUT2 (1 << 3)
#define SERIALS_MODEM_LOOPBACK (1 << 4)

#define SERIALS_INTERRUPT_WHEN_DATA_AVAILABLE (1 << 0)
#define SERIALS_INTERRUPT_WHEN_TRANSMITTER_EMPTY (1 << 1)
#define SERIALS_INTERRUPT_WHEN_BREAK_EMPTY (1 << 2)
#define SERIALS_INTERRUPT_WHEN_STATUS_UPDATE (1 << 3)

#define SERIALS_LINE_DATA_READY (1 << 0)
#define SERIALS_LINE_OVERRUN_ERROR (1 << 1)
#define SERIALS_LINE_PARITY_ERROR (1 << 2)
#define SERIALS_LINE_FRAMING_ERROR (1 << 3)
#define SERIALS_LINE_BREAK_INDICATOR (1 << 4)
#define SERIALS_LINE_TRANSMITTER_BUF_EMPTY (1 << 5)
#define SERIALS_LINE_TRANSMITTER_EMPTY (1 << 6)
#define SERIALS_LINE_IMPENDING_ERROR (1 << 7)

#define SERIALS_ENABLE_FIFO (1 << 0)
#define SERIALS_FIFO_CLEAR_RECEIVE (1 << 1)
#define SERIALS_FIFO_CLEAR_TRANSMIT (1 << 2)
#define SERIALS_FIFO_ENABLE_64_BYTE (1 << 5)
#define SERIALS_FIFO_TRIGGER_LEVEL1 (0 << 6)
#define SERIALS_FIFO_TRIGGER_LEVEL2 (1 << 6)
#define SERIALS_FIFO_TRIGGER_LEVEL3 (2 << 6)
#define SERIALS_FIFO_TRIGGER_LEVEL4 (3 << 6)

namespace artemis
{
namespace devices
{
void SerialDevice::WriteRegister(std::uint16_t reg, std::uint8_t val)
{
	arch::Outp(this->port_ + reg, val);
}

std::uint8_t SerialDevice::ReadRegister(std::uint16_t reg)
{
	return arch::Inp<std::uint8_t>(this->port_ + reg);
}

int SerialDevice::Write(std::string_view string)
{
	for(auto& ch: string)
	{
		this->Write(ch);
	}

	return string.length();
}

void SerialDevice::Write(std::uint8_t ch)
{
	while(!(this->ReadRegister(SERIALS_LINE_STATUS) & SERIALS_LINE_TRANSMITTER_BUF_EMPTY))
	{
		arch::Pause();
	}

	this->WriteRegister(SERIALS_DATA, ch);
}

bool SerialDevice::Initialize()
{
	this->WriteRegister(SERIALS_INTERRUPT_IDENTIFACTOR, 0);

	// Enabled DLAB and set baud rate divisor
	this->WriteRegister(SERIALS_LINE_CONTROL, SERIALS_LINE_DLAB_STATUS);
	this->WriteRegister(SERIALS_BAUD_RATE_LOW, 3);
	this->WriteRegister(SERIALS_BAUD_RATE_HIGH, 0);

	// Configure for 8 bits, no parity, and one stop bit
	this->WriteRegister(SERIALS_LINE_CONTROL, SERIALS_LINE_DS_8);

	// Enable FIFO, clear them with 14-byte threshold
	this->WriteRegister(SERIALS_FIFO_CONTROLLER, SERIALS_ENABLE_FIFO | SERIALS_FIFO_CLEAR_RECEIVE |
													 SERIALS_FIFO_CLEAR_TRANSMIT |
													 SERIALS_FIFO_TRIGGER_LEVEL4);

	// Enable IRQs, set RTS/DSR, and set in loopback mode
	this->WriteRegister(SERIALS_MODEM_CONTROL, SERIALS_MODEM_RTS | SERIALS_MODEM_DTR |
												   SERIALS_MODEM_OUT2 | SERIALS_MODEM_LOOPBACK);

	// Test Serial chip by sending one byte
	this->WriteRegister(SERIALS_DATA, 0xae);

	if(this->ReadRegister(SERIALS_DATA) != 0xae)
	{
		return false;
	}

	this->WriteRegister(SERIALS_MODEM_CONTROL, SERIALS_MODEM_RTS | SERIALS_MODEM_DTR |
												   SERIALS_MODEM_OUT1 | SERIALS_MODEM_OUT2);

	return true;
}
} // namespace devices
} // namespace artemis