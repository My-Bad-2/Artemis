#include "limine.h"
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

__SECTION(".limine_requests")
volatile limine_paging_mode_request paging_mode_request = {
	.id = LIMINE_PAGING_MODE_REQUEST,
	.revision = 0,
	.response = nullptr,
	.mode = LIMINE_PAGING_MODE_MAX,
	.max_mode = LIMINE_PAGING_MODE_MAX,
	.min_mode = LIMINE_PAGING_MODE_MIN,
};

__SECTION(".limine_requests")
volatile limine_kernel_address_request kernel_address_request = {
	.id = LIMINE_KERNEL_ADDRESS_REQUEST,
	.revision = 0,
	.response = nullptr,
};

__SECTION(".limine_requests")
volatile limine_kernel_file_request kernel_file_request = {
	.id = LIMINE_KERNEL_FILE_REQUEST,
	.revision = 0,
	.response = nullptr,
};

__SECTION(".limine_requests_end_marker") __USED static volatile LIMINE_REQUESTS_END_MARKER;

__CDECLS_END