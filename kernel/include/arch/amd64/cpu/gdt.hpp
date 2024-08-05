#ifndef AMD64_CPU_GDT_HPP
#define AMD64_CPU_GDT_HPP

#include <cstdint>
#include <array>
#include <sys/defs.h>

namespace artemis
{
namespace cpu
{
enum GdtFlags : std::uint8_t
{
	FlagAccess = (1 << 0),
	FlagRw = (1 << 1),
	FlagDc = (1 << 2),
	FlagExecutable = (1 << 3),
	FlagSystem = (1 << 4),
	FlagDpl3 = (3 << 5),
	FlagPresent = (1 << 7),

	LongMode = (1 << 1),
	FlagDB = (1 << 2),
	FlagGranularity = (1 << 3),
};

struct Tss
{
	std::uint32_t reserved_1;
	std::array<uint64_t, 3> rsp;
	std::uint64_t reserved_2;
	std::array<std::uint64_t, 7> ist;
	std::uint64_t reserved_3;
	std::uint16_t reserved_4;
	std::uint16_t iopb;

	constexpr Tss() = default;
} __PACKED;

struct GdtSegment
{
	std::uint16_t limit_low;
	std::uint16_t base_low;
	std::uint8_t base_mid;
	std::uint8_t access;
	std::uint8_t limit_high : 4;
	std::uint8_t granularity : 4;
	std::uint8_t base_high;

	constexpr GdtSegment() = default;

	void Write(std::uint32_t base, std::uint32_t limit, std::uint8_t granularity,
			   std::uint8_t access);

	inline void Write(std::uint8_t granularity, std::uint8_t access)
	{
		this->Write(0, 0xffffffff, granularity, access);
	}

	inline void Write()
	{
		this->Write(0, 0, 0, 0);
	}
} __PACKED;

struct TssSegment
{
	std::uint16_t len;
	std::uint16_t base_low;
	std::uint8_t base_mid;
	std::uint8_t flags_low;
	std::uint8_t flags_high;
	std::uint8_t base_high;
	std::uint32_t base_upper;
	std::uint32_t reserved;

	constexpr TssSegment() = default;

	void Write(Tss* tss);
} __PACKED;

struct GdtTable
{
	GdtSegment null;
	GdtSegment kernel_code;
	GdtSegment kernel_data;
	GdtSegment user_data;
	GdtSegment user_code;
	TssSegment tss;

	constexpr GdtTable() = default;
};

struct GdtRegister
{
	std::uint16_t limit;
	std::uint64_t base;

	constexpr GdtRegister() = default;

	constexpr GdtRegister(std::uint16_t limit, std::uint64_t base) : limit(limit), base(base)
	{
	}
} __PACKED;

class Gdt
{
  public:
	constexpr Gdt() = default;

	constexpr Gdt(std::uint16_t cpu_id) : cpu_id_(cpu_id)
	{
	}

	~Gdt() = default;

	Gdt(const Gdt&) = delete;
	Gdt(Gdt&&) = delete;

	Gdt& operator=(const Gdt&) = delete;
	Gdt& operator=(Gdt&&) = delete;

	void Initialize();
	void Load();

	std::uint16_t get_cpu_id()
	{
		return this->cpu_id_;
	}

  private:
	std::uint16_t cpu_id_;
	GdtTable gdt_table_;
	Tss tss_;
};
} // namespace cpu
} // namespace artemis

#endif // AMD64_CPU_GDT_HPP