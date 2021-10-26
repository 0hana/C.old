/* Hanami Zero (C) 2021: memory/copy.c */
#include "0hana/memory.h"

v copy(x Size, v c * c Original, o c Copy) { while(Size-- > 0) { ((e*)Copy)[Size] = ((e*)Original)[Size]; } }


#ifdef  test

test {
	#define Number 0x10000

	int Original[Number];
	int     Copy[Number];

	for(x X = 0; X < Number; X++) {
		Original[X] = X * X;
		    Copy[X] =     0;
	}

	copy(s(int) * Number, Original, Copy);

	for(x X = 0; X < Number; X++) {
		if(subtest(Copy[X] == Original[X], "check loop iteration: %lu", X));
		else return;
	}
}

#endif//test
