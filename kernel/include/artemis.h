#ifndef ARTEMIS_H
#define ARTEMIS_H

#include <stddef.h>
#include <limine.h>
#include <sys/defs.h>

__CDECLS_BEGIN

extern volatile limine_memmap_request memmap_request;
extern volatile limine_hhdm_request hhdm_request;

static inline size_t HHDMOffset(void)
{
	return hhdm_request.response->offset;
}

__CDECLS_END

#endif // ARTEMIS_H