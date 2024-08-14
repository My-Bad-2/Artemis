#ifndef AMD64_CPU_IDT_HPP
#define AMD64_CPU_IDT_HPP

#include <array>
#include <drivers/interrupt.hpp>
#include <sys/defs.h>

#define MAX_ENTRIES 256

namespace artemis
{
namespace cpu
{
enum IdtEntryType : std::uint8_t
{
	InterruptGate = 0xe,
	TrapGate = 0xf,
};

enum IdtDpl : std::uint8_t
{
	IdtDpl0,
	IdtDpl1,
	IdtDpl2,
	IdtDpl3,
};

struct IdtSegment
{
	std::uint16_t offset_low;
	std::uint16_t selector;
	std::uint8_t ist;
	std::uint8_t type_attributes;
	std::uint16_t offset_mid;
	std::uint32_t offset_high;
	std::uint32_t reserved;

	constexpr IdtSegment() = default;

	void Write(std::uintptr_t handler, std::uint8_t ist, IdtEntryType type, IdtDpl dpl,
			   std::uint16_t selector);
} __PACKED;

struct IdtTable
{
	std::array<IdtSegment, MAX_ENTRIES> entries;

	constexpr IdtTable() = default;

	IdtSegment& operator[](std::size_t index)
	{
		return this->entries[index];
	}
};

struct IdtRegister
{
	std::uint16_t limit;
	std::uint64_t base;

	constexpr IdtRegister() = default;

	IdtRegister(std::uint16_t limit, std::uint64_t base) : limit(limit), base(base)
	{
	}
} __PACKED;

class Idt
{
  public:
	Idt() = default;

	Idt(const Idt&) = delete;
	Idt(Idt&&) = delete;

	Idt& operator=(const Idt&) = delete;
	Idt& operator=(Idt&&) = delete;

	~Idt() = default;

	void Initialize();
	void Load();

	bool HandleInterrupts(Iframe* iframe);

  private:
	IdtTable idt_table_;
	std::array<Interrupt, MAX_ENTRIES> handlers_;
};
} // namespace cpu
} // namespace artemis

#endif // AMD64_CPU_IDT_HPP