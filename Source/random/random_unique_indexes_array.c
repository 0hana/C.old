//Zero Hanami (C) 2021: random_unique_indexes_array.c

#include "0hana/random.h"

size_t * random_unique_indexes_array(size_t const Indexes, size_t Variation) {
	size_t * const Array = (size_t *)nalloc(sizeof(size_t) * Indexes);
	if(!Array) return NULL;

	size_t * const Card = (size_t *)nalloc(sizeof(size_t) * Indexes);
	if(!Card) return (size_t *)(free(Array), NULL);

	Array[0] = 0;
	 Card[0] = 0;
	for(size_t I = 1; I < Indexes; I++) {
		Array[I] = 1;
		 Card[I] = I;
	}

	for(size_t I = Indexes, R, V; (I > 0) & (Variation > 0); Card[R] = Card[--I]) {
		R = random % I;
		V = random % (Variation + 1);
		Variation -= V;
		Array[Card[R]] += V;
	}

	for(size_t I = 1; I < Indexes; I++) Array[I] += Array[I - 1];

	return (size_t *)(free(Card), Array);
}
