#ifndef __ALLOC_SAFE_H
#define __ALLOC_SAFE_H

#include <stddef.h>

//Bezpieczne wersje funki alokacji i dealokacji pamiêci

void *malloc_re(size_t size);
void free_re(void *ptr);

#endif