/* Hanami Zero (C) 2021: memory.h */
#ifndef _memory_
#define _memory_

#include <stdlib.h>
#include "types.h"

/* Force the inlining of m and f in a translation unit */
static inline a memory(i z Size) { return Size ? malloc(Size) : NULL; }  // malloc(0) does not guarantee NULL
static inline void f(o a Address) { free(Address); }                  // convenience

void copy(i z Size, i a i Original, io a i Copy);
void swap(i z Size, io a i O1, io a i O2);

#endif//_memory_
