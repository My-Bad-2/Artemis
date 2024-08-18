#ifndef AMD64_MMU_HPP
#define AMD64_MMU_HPP

#include <cstddef>

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

enum PageFlags : std::size_t {
	PageFlagPresent = (1 << 0),
	PageFlagWritable = (1 << 1),
	PageFlagUserAccessible = (1 << 2),
	PageFlagWriteThrough = (1 << 3),
	PageFlagNoCache = (1 << 4),
	PageFlagAccessed = (1 << 5),
	PageFlagDirty = (1 << 6),
	PageFlagHugePage = (1 << 7),
	PageFlagGlobal = (1 << 8),
	PageFlagNoExecute = (1UL << 63),
};
} // namespace artemis

#endif // AMD64_MMU_HPP