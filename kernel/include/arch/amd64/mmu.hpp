#ifndef AMD64_MMU_HPP
#define AMD64_MMU_HPP

namespace artemis
{
enum PageFaultErrorCode
{
	PageFaultPresent = (1 << 0),
	PageFaultWrite = (1 << 1),
	PageFaultUser = (1 << 2),
	PageFaultReserveWrite = (1 << 3),
	PageFaultInstructionFetch = (1 << 4),
	PageFaultProtectionKey = (1 << 5),
    PageFaultShadowStack = (1 << 6),
	PageFaultSoftwareExten = (1 << 15),
};
} // namespace artemis

#endif // AMD64_MMU_HPP