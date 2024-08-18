#ifndef ARTEMIS_H
#define ARTEMIS_H

#include <stddef.h>
#include <limine.h>
#include <sys/defs.h>

__CDECLS_BEGIN

extern volatile limine_memmap_request memmap_request;
extern volatile limine_hhdm_request hhdm_request;
extern volatile limine_paging_mode_request paging_mode_request;
extern volatile limine_kernel_address_request kernel_address_request;
extern volatile limine_kernel_file_request kernel_file_request;

static inline size_t HHDMOffset(void)
{
	return hhdm_request.response->offset;
}

static inline bool IsPagingModeMax()
{
	return paging_mode_request.response->mode == LIMINE_PAGING_MODE_MAX;
}

__CDECLS_END

#endif // ARTEMIS_H