#include <sys/defs.h>

__CDECLS_BEGIN

typedef void (*func_ptr)();

extern func_ptr _init_array_start[0], _init_array_end[0];
extern func_ptr _fini_array_start[0], _fini_array_end[0];

void _init()
{
	for(func_ptr* func = _init_array_start; func != _init_array_end; func++)
	{
		(*func)();
	}
}

void _fini()
{
	for(func_ptr* func = _fini_array_start; func != _fini_array_end; func++)
	{
		(*func)();
	}
}

func_ptr _init_array_start[0] __USED __SECTION(".init_array") __ALIGNED(sizeof(func_ptr)) = {};
func_ptr _fini_array_start[0] __USED __SECTION(".fini_array") __ALIGNED(sizeof(func_ptr)) = {};

__CDECLS_END