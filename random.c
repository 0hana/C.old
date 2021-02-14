//Zero Hanami (C) 2021: random.c

#include "random.h"

size_t * random_array_of_unique_indices(size_t * Random, size_t const Modulus, size_t const Indices) {
	//Check for parameter error or preallocated return array
	if(Indices > Modulus || !(Random = Random ? Random : malloc(sizeof(size_t) * Indices))) return NULL;

	/*
		I'm kind of surprised I didn't think of this yesterday, but thanks
		Shane for listening to me talk the problem out in terms of playing
		cards -- now we have a good random unique integer array generator.

		The deck of cards solves the final outcome distribution bias by
		randomly selecting the order in which the indices are selected.

		-- Zero
	*/

	size_t N = Indices, M = Modulus, Card[Indices];

	//Generate bounded random increments
	for(size_t I = 0; I < Indices; I++) Card[I] = I;
	while(N > 0) {
		size_t R = (unsigned)rand() % N;
		Random[Card[R]] = (unsigned)rand() % (M - N + 1) + 1;
		M -= Random[Card[R]];
		Card[R] = Card[--N];
	}

	//Sum the increments ascending to generate the random selection list
	Random[0] -= 1;
	for(N = 1; N < Indices; N++) Random[N] = Random[N] + Random[N - 1];

	return Random;
}
