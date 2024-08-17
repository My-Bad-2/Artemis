#include <artemis.h>
#include <sys/defs.h>

__CDECLS_BEGIN

__SECTION(".limine_requests") __USED static volatile LIMINE_BASE_REVISION(2);

__SECTION(".limine_requests_start_marker") __USED static volatile LIMINE_REQUESTS_START_MARKER;

__SECTION(".limine_requests")
volatile limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
	.response = nullptr,
};

__SECTION(".limine_requests")
volatile limine_hhdm_request hhdm_request = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0,
	.response = nullptr,
};

__SECTION(".limine_requests_end_marker") __USED static volatile LIMINE_REQUESTS_END_MARKER;

__CDECLS_END