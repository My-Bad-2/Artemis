#include <cpu/gdt.hpp>
#include <cstdint>

namespace artemis
{
namespace cpu
{
extern "C" void LoadGdt(GdtRegister* gdtr);
extern "C" void LoadTss(void);

void GdtSegment::Write(std::uint32_t base, std::uint32_t limit, std::uint8_t granularity,
					   std::uint8_t access)
{
	this->limit_low = static_cast<std::uint16_t>(limit & 0xffff);
	this->base_low = static_cast<std::uint16_t>(base & 0xffff);
	this->base_mid = static_cast<std::uint8_t>((base >> 16) & 0xff);
	this->access = access;
	this->limit_high = (limit >> 16) & 0x0f;
	this->granularity = granularity;
	this->base_high = static_cast<std::uint8_t>((base >> 24) & 0xff);
}

void TssSegment::Write(Tss* tss)
{
	const std::uintptr_t tss_addr = reinterpret_cast<std::uintptr_t>(tss);

	this->len = sizeof(Tss);
	this->base_low = static_cast<std::uint16_t>(tss_addr & 0xffff);
	this->base_mid = static_cast<std::uint8_t>((tss_addr >> 16) & 0xff);
	this->flags_low = 0x89;
	this->flags_high = 0;
	this->base_high = static_cast<std::uint8_t>((tss_addr >> 24) & 0xff);
	this->base_upper = static_cast<std::uint32_t>(tss_addr >> 32);
	this->reserved = 0;
}

void Gdt::Initialize()
{
	constexpr std::uint8_t kCodeFlags = FlagPresent | FlagSystem | FlagExecutable | FlagRw;
	constexpr std::uint8_t kDataFlags = FlagPresent | FlagSystem | FlagRw;

	this->gdt_table_.null.Write();
	this->gdt_table_.kernel_code.Write(LongMode, kCodeFlags);
	this->gdt_table_.kernel_data.Write(0, kDataFlags);
	this->gdt_table_.user_data.Write(0, kDataFlags | FlagDpl3);
	this->gdt_table_.user_code.Write(LongMode, kCodeFlags | FlagDpl3);
	this->gdt_table_.tss.Write(&this->tss_);
}

void Gdt::Load()
{
	struct GdtRegister gdtr =
		GdtRegister(sizeof(GdtTable) - 1, reinterpret_cast<std::uint64_t>(&this->gdt_table_));

    LoadGdt(&gdtr);
    LoadTss();
}
} // namespace cpu
} // namespace artemis