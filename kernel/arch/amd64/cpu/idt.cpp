#include <asm/regs.h>
#include <mmu.hpp>
#include <cpu/idt.hpp>
#include <cpu/interrupts.hpp>
#include <cpu/gdt.hpp>
#include <cpu/registers.hpp>
#include <logger.hpp>
#include <cstddef>
#include <libs/stacktrace.hpp>

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

void DumpPageFaultError(Iframe* iframe, std::uintptr_t cr2)
{
	std::uint64_t error_code = iframe->err_code;

	std::uintptr_t virt_addr = cr2;
	std::uintptr_t ssp = iframe->user_ss & 0xffffffff;
	std::uintptr_t sp = iframe->user_sp;
	std::uintptr_t cs = iframe->cs & 0xffffffff;
	std::uintptr_t ip = iframe->ip;

	LogFatal("<PAGE FAULT> Instruction Pointer  = 0x%lx:0x%lx", cs, ip);
	LogFatal("<PAGE FAULT> Stack Pointer        = 0x%lx:0x%lx", ssp, sp);
	LogFatal("<PAGE FAULT> Fault Linear Address = 0x%lx", virt_addr);
	LogFatal("<PAGE FAULT> Error Code Value     = 0x%lx", error_code);
	LogFatal("<PAGE FAULT> Error Code Type      = %s %s %s%s %s, %s",
			 error_code & PageFaultUser ? "user" : "supervisor",
			 error_code & PageFaultWrite ? "write" : "read",
			 error_code & PageFaultInstructionFetch ? "instruction" : "data",
			 error_code & PageFaultReserveWrite ? "reserved write" : "",
			 error_code & PageFaultShadowStack ? "shadow stack" : "",
			 error_code & PageFaultPresent ? "protection violation" : "page not present");
}

void PageFaultHandler(Iframe* iframe, std::uintptr_t cr2)
{
	DumpPageFaultError(iframe, cr2);
	std::uint64_t error_code = iframe->err_code;

	if(error_code & PageFaultUser)
	{
		switch(error_code)
		{
			case 4:
			case 5:
			case 6:
			case 7:
				LogPanic("User Page Fault exception, halting!");
		}
	}
	else
	{
		switch(error_code)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				LogPanic("Supervisor Page Fault exception, halting!");
		}
	}

	LogPanic("Unhandled Page Fault exception, halting!");
}

void DumpInterruptFrame(Iframe* iframe)
{
	LogFatal("CS : %#08lx RIP: %0#lx  EFL: %#08lx", iframe->cs, iframe->ip, iframe->flags);
	LogFatal("RAX: %#08lx RBX: %#08lx RCX: %#08lx", iframe->rax, iframe->rbx, iframe->rcx);
	LogFatal("RDX: %#08lx RSI: %#08lx RBP: %#08lx", iframe->rdx, iframe->rsi, iframe->rbp);
	LogFatal("RSP: %#08lx R8 : %#08lx R9 : %#08lx", iframe->rsi, iframe->r8, iframe->r9);
	LogFatal("R10: %#08lx R11: %#08lx R12: %#08lx", iframe->r10, iframe->r11, iframe->r12);
	LogFatal("R13: %#08lx R14: %#08lx R15: %#08lx", iframe->r13, iframe->r14, iframe->r15);
	LogFatal("EC : %#08lx USP: %#08lx USS: %#08lx", iframe->err_code, iframe->user_sp,
			 iframe->user_ss);
}

void ExceptionHandler(Iframe* iframe)
{
	DumpStackTrace();

	if(iframe->vector == IntPageFault)
	{
		std::uint64_t cr2 = ReadCr2();
		return PageFaultHandler(iframe, cr2);
	}

	DumpInterruptFrame(iframe);
	LogPanic("Unhandled Exception %lu, halting!", iframe->vector);
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
	IdtEntryType type = InterruptGate;
	IdtDpl dpl;

	for(int i = 0; i < MAX_ENTRIES; i++)
	{
		switch(i)
		{
			case IntBreakpoint:
				dpl = IdtDpl3;
			default:
				dpl = IdtDpl0;
		}

		this->idt_table_[i].Write(isr_table[i], 0, type, dpl, offsetof(GdtTable, kernel_code));
		this->handlers_[i].set_vector(i);
	}

	for(int i = 0; i < IntPlatformBase; i++)
	{
		this->handlers_[i].Set([](Iframe* iframe) {
			details::ExceptionHandler(iframe);
		});
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

bool Idt::HandleInterrupts(Iframe* iframe)
{
	return this->handlers_[iframe->vector](iframe);
}
} // namespace cpu
} // namespace artemis