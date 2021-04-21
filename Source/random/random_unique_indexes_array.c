//Zero Hanami (C) 2021: random.c

#include "0hana/random.h"

size_t * random_unique_Indexes_array(size_t const Modulus, size_t const Indexes) {
	size_t * const Random = (size_t *)malloc(sizeof(size_t) * Indexes);
	if(!Random) return NULL;

	/*
		The deck of cards solves the final outcome distribution bias by
		randomly selecting the order in which the Indexes are selected.

		-- Zero
	*/

	size_t
	N = Indexes,
	M = Modulus,
	* const Card = (size_t *)malloc(sizeof(size_t) * Indexes);

	if(!Card) return (size_t *)(free(Random), NULL);

	//Generate bounded random increments
	for(size_t I = 0; I < Indexes; I++) Card[I] = I;
	while(N > 0) {
		size_t R = (unsigned)rand() % N;
		Random[Card[R]] = (unsigned)rand() % (M - N + 1) + 1;
		M -= Random[Card[R]];
		Card[R] = Card[--N];
	}

	//Sum the increments ascending to generate the random selection list
	Random[0] -= 1;
	for(N = 1; N < Indexes; N++) Random[N] = Random[N] + Random[N - 1];

	return Random;
}
