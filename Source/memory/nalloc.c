//Zero Hanami (C) 2021: nalloc.c

#include "0hana/memory.h"

void * nalloc(size_t const Size) {
	return Size ? malloc(Size) : NULL;
}
