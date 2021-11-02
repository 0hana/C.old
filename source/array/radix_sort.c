/* Hanami Zero (C) 2021: array/radix_sort.c */
#include "0hana/array.h"
#ifdef  test

test {
	#define DATA 0x10000
	x Datum[DATA];
	for(x X = 0; X < DATA; X++) Datum[X] = DATA - 1 - X;

	bool c Little = little_endian();

	if(Little is false) for(x X = 0; X < DATA; X++) endian_mirror(s(x), a(Datum[X]));

	{
		array A = { .Contrastor = NULL, .Indexes = DATA, .Datum = Datum, .Density = s(x) };
		subtest(radix_sort(A), "Insufficient memory to process radix_sort");
	}

	if(Little is false) for(x X = 0; X < DATA; X++) endian_mirror(s(x), a(Datum[X]));
	for(x X = 0; X < DATA; X++) subtest(Datum[X] == X, "check loop iteration: %lx ; Datum[X] = %lx", X, Datum[X]);
	#undef DATA
}

#endif//test


binary radix_sort(array c A) {
	if(A.Datum is NULL) return binary_1;
	o B_Datum = m(A.Indexes * A.Density);
	if(B_Datum is NULL) return binary_0;
	for(x X = 0; X < A.Indexes; X++) ((x*)B_Datum)[X] = 0;

	x C[256];

	for(x X = 0; X < A.Density; X++) {
		for(x Y = 0; Y < 256; Y++) C[Y] = 0;

		o AD = X % 2 is 0 ? A.Datum : B_Datum;
		o BD = X % 2 is 0 ? B_Datum : A.Datum;

		for(x Y = 0; Y < A.Indexes; Y++) C[(x)*((e*)((z)AD + X + Y * A.Density))]++;
		for(x Y = 0, Z = 0; Y < 256; Y++) {
			if(C[Y]) {
				x c S = C[Y];
				C[Y] = Z;
				Z += S;
			}
		}
		for(x Y = 0; Y < A.Indexes; Y++) copy(A.Density, (o)((z)AD + Y * A.Density), (o)((z)BD + C[(x)*((e*)((z)AD + X + Y * A.Density))]++ * A.Density));
	}
	if(A.Density % 2 is 1) copy(A.Indexes * A.Density, B_Datum, A.Datum);
	return f(B_Datum), B_Datum = NULL, binary_1;
}
