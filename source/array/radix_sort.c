/* Hanami Zero (C) 2021: array/radix_sort.c */
#include "0hana/array.h"
#ifdef  test

test {
	{
		#define DATA 0x10000
		size_t Datum[DATA];
		for(size_t X = 0; X < DATA; X++) Datum[X] = DATA - 1 - X;

		if( ! little_endian ) for(size_t X = 0; X < DATA; X++) endian_mirror(sizeof(size_t), &(Datum[X]));

		{
			array A = { .Contrastor = NULL, .Indexes = DATA, .Datum = Datum, .Density = sizeof(size_t) };
			subtest(radix_sort(A) returns no_error, "Insufficient memory to process radix_sort");
		}

		if( ! little_endian ) for(size_t X = 0; X < DATA; X++) endian_mirror(sizeof(size_t), &(Datum[X]));
	
		for(size_t X = 0; X < DATA; X++) subtest(Datum[X] == X, "check loop iteration: %lx ; Datum[X] = %lx", X, Datum[X]);
		#undef  DATA
	}

	{
		#define DATA (26 * 26 * 26)
		char Datum[DATA][3];
		for(size_t X = 0; X < DATA; X++) {
			Datum[X][0] = 'z' - (X / (26 * 26)) % 26;
			Datum[X][1] = 'z' - (X / 26) % 26;
			Datum[X][2] = 'z' - X % 26;
		}

		/*
			zzz, zzy, zzx, ...
			zyz, zyy, zyx, ...
			yzz, yzy, yzx, ...
			azz, azy, azx, ...
			aaz, aay, aax, ... aaa
		*/
		
		/* Encoded in big endian */ for(size_t X = 0; X < DATA; X++) endian_mirror(3, &(Datum[X]));

		{
			array A = { .Contrastor = NULL, .Indexes = DATA, .Datum = Datum, .Density = 3 };
			subtest(radix_sort(A) returns no_error, "Insufficient memory to process radix_sort");
		}

		/* Encoded in big endian */ for(size_t X = 0; X < DATA; X++) endian_mirror(3, &(Datum[X]));

		for(size_t X = 0; X < DATA; X++) {
			char character[3] = {
				'a' + (X / (26 * 26)) % 26,
				'a' + (X / 26) % 26,
				'a' + X % 26
			};

			/*
			  Using the above expressions (defining character[0], [1], [2]) in subtest directly causes
			  the gcc C compiler to issue a -Wformat= error since the modulus operators (%) are stringerized
			  within the subtest expression, and passed to fprintf, which normally uses '%' followed by another character
			  to decide how to treat additional parameters following the format string.

			  To avoid this issue, I simply evaluate the expressions in advance, and use the values, as character[X],
			  within the subtest expression field.
			*/

			subtest(Datum[X][0] == character[0], "check loop iteration: %lu ; Datum[X][0] = '%c'", X, Datum[X][0]);
			subtest(Datum[X][1] == character[1], "check loop iteration: %lu ; Datum[X][1] = '%c'", X, Datum[X][1]);
			subtest(Datum[X][2] == character[2], "check loop iteration: %lu ; Datum[X][2] = '%c'", X, Datum[X][2]);
		}
		#undef  DATA
	}
}

#endif//test


function radix_sort(i array A) {
	if(A.Indexes < 2) return no_error;  // Sorting is not relevant without 2 or more datum
	a B_Datum = memory(A.Indexes * A.Density);

	if(B_Datum == NULL) return no_memory;
	for(z X = 0; X < A.Indexes * A.Density; X++) ((byte*)B_Datum)[X] = 0;

	z C[256];

	for(z X = 0; X < A.Density; X++) {
		for(z Y = 0; Y < 256; Y++) C[Y] = 0;

		a AD = X % 2 == 0 ? A.Datum : B_Datum;
		a BD = X % 2 == 0 ? B_Datum : A.Datum;

		for(z Y = 0; Y < A.Indexes; Y++) C[(z)*((byte*)((uintptr_t)AD + X + Y * A.Density))]++;
		for(z Y = 0, Z = 0; Y < 256; Y++) {
			if(C[Y]) {
				i z S = C[Y];
				C[Y] = Z;
				Z += S;
			}
		}
		for(z Y = 0; Y < A.Indexes; Y++)
			copy(
				A.Density,
				(a)((uintptr_t)AD + Y * A.Density),
				(a)((uintptr_t)BD + C[(z)*((byte*)((uintptr_t)AD + X + Y * A.Density))]++ * A.Density));
	}

	if(A.Density % 2 == 1) copy(A.Indexes * A.Density, B_Datum, A.Datum);
	free(B_Datum);
	B_Datum = NULL;
	return no_error;
}
