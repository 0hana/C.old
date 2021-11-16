/* Hanami Zero (C) 2021: memory/copy.c */
#include "0hana/memory.h"

#ifdef  test

test {
	#define Number 0x10000

	int Original[Number];
	int     Copy[Number];

	for(size_t X = 0; X < Number; X++) {
		Original[X] = X * X;
		    Copy[X] =     0;
	}

	copy(s(int) * Number, Original, Copy);

	for(size_t X = 0; X < Number; X++) {
		if(subtest(Copy[X] == Original[X], "check loop iteration: %lu", X));
		else return;
	}
	#undef  Number
}

#endif//test


void copy(i z Size, i a i Original, io a i Copy) {
	for(z X = 0; X < Size; X++)
		((byte*)Copy)[X] = ((byte*)Original)[X];
}
