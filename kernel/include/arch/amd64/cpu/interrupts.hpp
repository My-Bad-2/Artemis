#ifndef AMD64_CPU_INTERRUPTS_HPP
#define AMD64_CPU_INTERRUPTS_HPP

namespace artemis
{
enum InterruptVector
{
	IntDivide0 = 0,
	IntDebug,
	IntNmi,
	IntBreakpoint,
	IntOverflow,
	IntBoundRange,
	IntInvalidOp,
	IntDeviceNa,
	IntDoubleFault,
	IntInvalidTss = 0xa,
	IntSegmentNotPresent,
	IntStackFault,
	IntGpFault,
	IntPageFault,
	IntReserved,
	IntFpuFpError,
	IntAlignmentCheck,
	IntMachineCheck,
	IntSimdFpError,
	IntVirt,

	IntMaxIntelDefined = 0x1f,
	IntPlatformBase = 0x20,
	IntPlatformMax = 0xef,

	IntLocalApicBase = 0xf0,
	IntApicSpurious = 0xf0,
	IntApicTimer,
	IntApicError,
	IntApicPmi,
	IntIpiGeneric,
	IntIpiReschedule,
	IntIpiInterrupt,
	IntIpiHalt,

	IntMax = 0xff,
	IntCount,
};
} // namespace artemis

#endif // AMD64_CPU_INTERRUPTS_HPP