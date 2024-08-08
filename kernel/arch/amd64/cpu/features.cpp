#include <cpu/features.hpp>

namespace artemis
{
namespace cpu
{
bool FeatureTest(CpuidBit bit)
{
	if((bit.word > 3) || (bit.bit > 31))
	{
		return false;
	}

	const CpuidLeaf leaf = ReadCpuid(bit.leaf_num, 0);

	switch(bit.word)
	{
		case kEax:
			return !!((1u << bit.bit) & leaf.Read(kEax));
		case kEbx:
			return !!((1u << bit.bit) & leaf.Read(kEbx));
		case kEcx:
			return !!((1u << bit.bit) & leaf.Read(kEcx));
		case kEdx:
			return !!((1u << bit.bit) & leaf.Read(kEdx));
		default:
			return false;
	}
}
} // namespace cpu
} // namespace artemis