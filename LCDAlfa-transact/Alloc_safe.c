#include <stdlib.h>
#include <util/atomic.h>
#include "Alloc_safe.h"

void *malloc_re(size_t size)
{
	void *ptr;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ptr=malloc(size);
	}
	return ptr;
}

void free_re(void *ptr)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		free(ptr);
	}
}
