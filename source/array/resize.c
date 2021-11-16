/* Hanami Zero (C) 2021: resize.c */
#include "0hana/array.h"

#ifdef  test

test {
	// Initialize
	array Array = { .Contrastor = NULL, .Indexes = 0, .Datum = NULL, .Density = sizeof(size_t) };
	array * const A = &(Array);

	// 0 to ~0 Failure allocation test
	{
		subtest(resize((size_t)_virtual_memory_space_-1, A) returns no_memory,
			"This subtest should always pass because allocating the entire memory space less 1 should always fail--unless constant and stack information is stored in a separate memory space--the assumption here is a virtual unified Von Neumann macro memory space. If you are reading this in a log file, this assumption has failed.");

		subtest(Array.Contrastor  == NULL); subtest(Array.Indexes == 0);
		subtest(Array.Datum       == NULL); subtest(Array.Density == sizeof(size_t));
	}

	// 0 to ~0 Successful allocation test
	{
		subtest(resize(0x10, A) returns no_error,
			"0 to ~0 Memory allocation failure");  // The system must already be near or at its limits to fail this subtest

		subtest(Array.Contrastor == NULL); subtest(Array.Indexes == 0x10);
		subtest(Array.Datum      != NULL); subtest(Array.Density == sizeof(size_t));
	}

	// Address Access Assignment
	for(size_t X = 0; X < 0x10; X++) {
		((size_t*)A->Datum)[X] = X;
		subtest(((size_t*)Array.Datum)[X] == X, "\"Memory assignment\" check loop iteration: %lu", X);
	}

	// ~0 to ~0 Failure reallocation test
	{
		a Datum_Copy = Array.Datum;

		subtest(resize((size_t)_virtual_memory_space_-1, A) returns no_memory,
			"This subtest should always pass because allocating the entire memory space less 1 should always fail--unless constant and stack information is stored in a separate memory space--the assumption here is a virtual unified Von Neumann macro memory space. If you are reading this in a log file, this assumption has failed.");

		subtest(Array.Contrastor  == NULL); subtest(Array.Indexes == 0x10);
		subtest(Array.Datum == Datum_Copy); subtest(Array.Density == sizeof(size_t));
	}

	// ~0 to ~0 Successful allocation test
	{
		subtest(resize(0x20, A) returns no_error,
			"~0 to ~0 Memory reallocation failure");  // The system must already be near or at its limits to fail this subtest

		subtest(Array.Contrastor == NULL); subtest(Array.Indexes == 0x20);
		subtest(Array.Datum      != NULL); subtest(Array.Density == sizeof(size_t));
	}

	// Deallocation test
	subtest(resize(0, A) returns no_error);  // Should NEVER fail if A is a valid, non-NULL array reference

	subtest(Array.Contrastor == NULL); subtest(Array.Indexes    == 0);
	subtest(Array.Datum      == NULL); subtest(Array.Density    == sizeof(size_t));
}

#endif//test


function resize(i z Indexes_Request, io array * i A) {
	// Memory release control
	if(Indexes_Request == 0) {
		if(A->Indexes == 0) return no_error;  // Nothing to do
		free(A->Datum);
		relax(A->Indexes) =    0;
		relax(A->Datum)   = NULL;
	}
	// Memory allocation control
	else
		if(A->Density != 0) {
			a Restore = A->Datum;
			//  0 to ~0 (m/alloc)
			if(A->Indexes == 0) {
				if((relax(A->Datum) = memory(A->Density * Indexes_Request)) == NULL) {
					relax(A->Datum) = Restore;
					return no_memory;  // Allocation failure
				}
			}
			// ~0 to ~0 (realloc)
			else
				if((relax(A->Datum) = realloc(A->Datum, A->Density * Indexes_Request)) == NULL) {
					relax(A->Datum) = Restore;
					return no_memory;  // Allocation failure
				}
			relax(A->Indexes) = Indexes_Request;
		}
	return no_error;  // Resizing success
}
