/* Hanami Zero (C) 2021: memory.h */
#ifndef _memory_
#define _memory_

#include <stdlib.h>
#include "types.h"

/* Force the inlining of m and f in a translation unit */
static inline o m(x c Size) { return Size ? malloc(Size) : NULL; }  // malloc(0) does not guarantee NULL
static inline v f(o c Address) { free(Address); }                   // convenience

v copy(x Size, v c * c Original, o c Copy);
v swap(x Size, o c O1, o c O2);

#endif//_memory_
