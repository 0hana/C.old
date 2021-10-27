/* Hanami Zero (C) 2021: array.h */
#ifndef _array_
#define _array_

#include "memory.h"

typedef struct array {
	ternary (* _c Contrastor)(v c * c O1, v c * c O2);
	x _c Indexes;
	o _c Datum;
	x _c Density;
} array;

#define index(number, Array) (*((Array).Datum + number * (Array).Density))

binary  reallocate_array(x c Indexes_Request, array * c A);  // attempt to reallocate the memory referenced A.Datum to hold A.Indexes * A.Density

binary  radix_sort(array c A);  // Expects data is stored in little endian order
binary  merge_sort(array c A);  /* Ascending comparison sort based on A.Contrastor -- if O1 is a lower index than O2, they will only be swapped if the A.Contrastor(O1, O2) returns ternary_1
                                   - This can be optimized by using only a greaterthan function */
  //  影 の 変換

/* The above 4 binary functions depend on a memory allocation attempt whose success cannot generally be known ahead of time.
   - A return of binary_1 indicates the allocation attempt failed and no mutation array data has been performed.
   - A return of binary_0 indicates the function completed successfully */

x ordinal_select(array c A, x c N);  // return the index of the Nth item as if the array were sorted in ascending order

#endif//_array_
