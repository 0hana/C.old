/* Hanami Zero (C) 2021: array.h */
#ifndef _array_
#define _array_

#include "memory.h"

typedef struct array {
	u(ternary, Contrastor, i a O1, i a O2);
	u(z, Indexes);
	u(a, Datum);
	u(z, Density);
} array;

function resize(i z Indexes_Request, io array * i A);
// attempt to reallocate the memory referenced A.Datum to hold A.Indexes * A.Density

function radix_sort(i array A);
// Expects data is stored in little endian order

function merge_sort(i array A);
/* Ascending comparison sort based on A.Contrastor --
   if O1 is a lower index than O2, they will only be swapped if the A.Contrastor(O1, O2) returns ternary_1
   - This can be optimized by using only a greaterthan function */

//  影 の 変換
#endif//_array_
