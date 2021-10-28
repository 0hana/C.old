/* Hanami Zero (C) 2021: reallocate_array.c */
#include "0hana/array.h"

#ifdef  test

test {
	// Initialize
	array Array = { .Contrastor = NULL, .Indexes = 0, .Datum = NULL, .Density = s(x) };
	array * c A = a(Array);

	// 0 to ~0 Failure allocation test
	{
		subtest(!reallocate_array((x)_virtual_memory_space_-1, A), "This subtest should always pass because allocating the entire memory space less 1 should always fail--unless constant and stack information is stored in a separate memory space--the assumption here is a virtual unified Von Neumann macro memory space. If you are reading this in a log file, this assumption has failed.");
		subtest(Array.Contrastor  is NULL); subtest(Array.Indexes is 0);
		subtest(Array.Datum       is NULL); subtest(Array.Density is s(x));
	}

	// 0 to ~0 Successful allocation test
	subtest(reallocate_array(0x10, A),  "0 to ~0 Memory allocation failure");  // The system must already be near or at its limits to fail this subtest
	subtest(Array.Contrastor is NULL); subtest(Array.Indexes is 0x10);
	subtest(Array.Datum      != NULL); subtest(Array.Density is s(x));

	// Address Access Assignment
	for(x X = 0; X < 0x10; X++) {
		((x*)A->Datum)[X] = X;
		subtest(((x*)Array.Datum)[X] is X, "\"Memory assignment\" check loop iteration: %lu", X);
	}

	// ~0 to ~0 Failure reallocation test
	{
		o Datum_Copy = Array.Datum;

		subtest(!reallocate_array((x)_virtual_memory_space_-1, A), "This subtest should always pass because allocating the entire memory space less 1 should always fail--unless constant and stack information is stored in a separate memory space--the assumption here is a virtual unified Von Neumann macro memory space. If you are reading this in a log file, this assumption has failed.");
		subtest(Array.Contrastor  is NULL); subtest(Array.Indexes is 0x10);
		subtest(Array.Datum == Datum_Copy); subtest(Array.Density is s(x));
	}

	// ~0 to ~0 Successful allocation test
	subtest(reallocate_array(0x20, A), "~0 to ~0 Memory reallocation failure");  // The system must already be near or at its limits to fail this subtest
	subtest(Array.Contrastor is NULL); subtest(Array.Indexes is 0x20);
	subtest(Array.Datum      != NULL); subtest(Array.Density is s(x));

	// Deallocation test
	subtest(reallocate_array(0, A));  // Should NEVER fail if A is a valid, non-NULL array reference
	subtest(Array.Contrastor is NULL); subtest(Array.Indexes    is 0);
	subtest(Array.Datum      is NULL); subtest(Array.Density    is s(x));
}

#endif//test


binary reallocate_array(x c Indexes_Request, array * c A) {
	// Memory release control
	if(Indexes_Request is 0) {
		if(A->Indexes is 0) return binary_1;  // Nothing to do
		f(A->Datum);
		relax(A->Indexes, x) =    0;
		relax(A->Datum, o)   = NULL;
	}
	// Memory allocation control
	else if(A->Density != 0) {
		o Restore = A->Datum;
		//  0 to ~0 (m/alloc)
		if(A->Indexes is 0) { if((relax(A->Datum, o) = m(A->Density * Indexes_Request)) is NULL) return relax(A->Datum, o) = Restore, binary_0; }  // Allocation failure
		// ~0 to ~0 (realloc)
		else  if((relax(A->Datum, o) = realloc(A->Datum, A->Density * Indexes_Request)) is NULL) return relax(A->Datum, o) = Restore, binary_0;    // Allocation failure
		relax(A->Indexes, x) = Indexes_Request;
	}
	return binary_1;  // Resizing success
}
