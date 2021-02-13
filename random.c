//Zero Hanami (C) 2021: random.c

#include "random.h"

size_t * random_array_of_unique_indices(size_t * Random, size_t const Indices, size_t const Modulus) {
	//Generate bounded random increments
	size_t N = Indices, M = Modulus;
	while(N-- > 0) {
		Random[N] = (unsigned)rand() % (M - N);
		M -= Random[N];
	}
	Random[0] -= 1;
	for(N = 1; N < Indices; N++) Random[N] = Random[N] + Random[N - 1];
	//Problem: combinations not evenly distributed -- attempt to codify combination ID numbers and derive integer sets from ID's instead
}
