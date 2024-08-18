#include <sys/defs.h>
#include <cstddef>

__CDECLS_BEGIN

static constexpr std::size_t atexit_max_funcs = 128;

struct AtexitFuncs
{
	void (*destructor_func)(void*);
	void* obj_ptr;
	void* dso_handle;
} __atexit_funcs[atexit_max_funcs];

unsigned __atexit_func_count = 0;
void* __dso_handle = nullptr;

int __cxa_atexit(void (*func)(void*), void* objptr, void* dso)
{
	if(__atexit_func_count >= atexit_max_funcs)
	{
		return -1;
	}

	__atexit_funcs[__atexit_func_count].destructor_func = func;
	__atexit_funcs[__atexit_func_count].obj_ptr = objptr;
	__atexit_funcs[__atexit_func_count].dso_handle = dso;
	__atexit_func_count++;

	return 0;
}

__CDECLS_END