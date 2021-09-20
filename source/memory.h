/* Hanami Zero (C) 2021: memory.h */
#ifndef _memory_
#define _memory_

#include <stdlib.h>
#include "types.h"

#define m(Size) (Size ? malloc(Size) : NULL)
#define f(Address) (free(Address))

v copy(x Size, o c Copy, v c * c Original);
v swap(x Size, o c O1, o c O2);

#endif//_memory_
