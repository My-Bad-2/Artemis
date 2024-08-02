#include <sys/defs.h>

__CDECLS_BEGIN

typedef void (*func_ptr)();

func_ptr _init_array_end[0] __USED __SECTION(".init_array") __ALIGNED(sizeof(func_ptr)) = {};
func_ptr _fini_array_end[0] __USED __SECTION(".fini_array") __ALIGNED(sizeof(func_ptr)) = {};

__CDECLS_END