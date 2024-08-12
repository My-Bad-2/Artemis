#include <cpu/idt.hpp>
#include <cpu/gdt.hpp>
#include <cstdint>
#include <utility>
#include <logger.hpp>

extern "C" std::uintptr_t isr_table[];

namespace artemis
{
namespace cpu
{
namespace details
{
inline std::uint8_t FetchIdtAttribute(IdtEntryType type, IdtDpl dpl)
{
	std::uint8_t ret = (1 << 7); // Present bit should be set
	ret |= (std::to_underlying(dpl) << 5);
	ret |= std::to_underlying(type);

	return ret;
}
} // namespace details

void IdtSegment::Write(std::uintptr_t handler, std::uint8_t ist, IdtEntryType type, IdtDpl dpl,
					   std::uint16_t selector)
{
	this->offset_low = static_cast<std::uint16_t>(handler & 0xffff);
	this->selector = selector;
	this->ist = ist;
	this->type_attributes = details::FetchIdtAttribute(type, dpl);
	this->offset_mid = static_cast<std::uint16_t>((handler >> 16) & 0xffff);
	this->offset_high = static_cast<std::uint32_t>((handler >> 32) & 0xffffffff);
	this->reserved = 0;
}

void Idt::Initialize()
{
	for(int i = 0; i < MAX_ENTRIES; i++)
	{
		this->idt_table_[i].Write(isr_table[i], 0, InterruptGate, IdtDpl0,
								  offsetof(GdtTable, kernel_code));
	}

	this->Load();
}

void Idt::Load()
{
	IdtRegister idtr = {
		sizeof(IdtTable) - 1,
		reinterpret_cast<std::uintptr_t>(&this->idt_table_),
	};

	asm volatile("lidt %0" ::"m"(idtr));
}
} // namespace cpu
} // namespace artemis

extern "C"
{
	void ExceptionHandler(void* rdi)
	{
		LogFatal("Interrupt detected!");
	}

	void NmiHandler()
	{
	}
}