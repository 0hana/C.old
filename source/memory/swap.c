/* Hanami Zero (C) 2021: memory/swap.c */
#include "0hana/memory.h"

v swap(x Size, o c O1, o c O2) {
	while(Size-- > 0) {
		e Copy         = ((e*)O2)[Size];
		((e*)O2)[Size] = ((e*)O1)[Size];
		((e*)O1)[Size] =           Copy;
	}
}


#ifdef test

test {
	#define Number 0x10000
	
	int Array_0[Number];
	int Array_1[Number];

	#define triangular_number(T) (int)((T * (T + 1)) / 2)
	#define     square_number(S) (int)(S * S)

	for(x X = 0; X < Number; X++) {
		Array_0[X] = triangular_number(X);
		Array_1[X] =     square_number(X);
	}

	//Initial swap
	swap(s(int) * Number, Array_0, Array_1);

	for(x X = 0; X < Number; X++) {
	#define loop_context_1 "\"Initial swap\" check loop iteration: %lu", X
		if(subtest(Array_0[X] ==     square_number(X), loop_context_1)
		&& subtest(Array_1[X] == triangular_number(X), loop_context_1));
		else return;
	}

	//Swap back (Symmetry test)
	swap(s(int) * Number, Array_0, Array_1);
	
	for(x X = 0; X < Number; X++) {
	#define loop_context_2 "\"Swap back\" check loop iteration: %lu", X
		if(subtest(Array_0[X] == triangular_number(X), loop_context_2)
		&& subtest(Array_1[X] ==     square_number(X), loop_context_2));
		else return;
	}

	//Reverse operand order swap (Commutivity test)
	swap(s(int) * Number, Array_1, Array_0);

	for(x X = 0; X < Number; X++) {
	#define loop_context_3 "\"Reverse operand order swap\" check loop: iteration %lu", X
		if(subtest(Array_0[X] ==     square_number(X), loop_context_3)
		&& subtest(Array_1[X] == triangular_number(X), loop_context_3));
		else return;
	}
}

#endif
