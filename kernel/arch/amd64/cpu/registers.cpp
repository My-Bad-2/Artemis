#include <cpu/registers.hpp>
#include <asm/regs.h>

namespace artemis
{
const std::size_t pat_value = (PatUncachable << 56) | (PatWriteBack << 48) |
							  (PatWriteProtect << 40) | (PatWriteThrough << 32) |
							  (PatWriteCombining << 24) | (PatForceUncachable << 16);

void EnablePat()
{
	WriteMsr(MSR_IA32_PAT, pat_value);
}
} // namespace artemis