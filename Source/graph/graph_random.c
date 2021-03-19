//Zero Hanami (C) 2021: graph_random.c

#include "graph.h"
#include "0hana/random/random.h"

struct graph * graph_random(size_t const Term_Limit, size_t const Link_Limit) {
	//Allocate return structures
	struct graph * Random = (struct graph *)malloc(sizeof(struct graph));
	if(!Random) return NULL;
	*(struct graph_term **)&Random->Term = (struct graph_term *)malloc(sizeof(struct graph_term) * (*(size_t *)&Random->Terms = (unsigned)rand() % Term_Limit));
	if(!Random->Term) return free(Random), NULL;
	else {
		
		//Determine number of edges per random vertex and Allocate edge array for each random vertex
		
		for(size_t V = 0; V < Random->Terms; V++) {
			*(struct graph_term_link **)&Random->Term[V].Link = (struct graph_term_link *)malloc(sizeof(struct graph_term_link) * (*(size_t *)&Random->Term[V].Links = (unsigned)rand() % Random->Terms % Link_Limit));
			if(!Random->Term[V].Link) {
				//Release all allocations prior to malloc failure
				while(V-- > 0) free(Random->Term[V].Link);
				return free(Random->Term), free(Random), NULL;
			}
			//Generate the random edge destinations in ascending order
			size_t Random_Term[Random->Term[V].Links], Minimum = 0;

			random_array_of_unique_indices(Random_Term, Random->Terms - 1, Random->Term[V].Links);
			//function will only fail on insufficient stack memory -- requires Random->Term[V].Links * sizeof(size_t) + constant to run successfully

			for(size_t E = 0; E < Random->Term[V].Links; E++) if((Random_Term[E] + V + 1) % Random->Terms < (Random_Term[Minimum] + V + 1) % Random->Terms) Minimum = E;
			for(size_t E = 0; E < Random->Term[V].Links; E++) {
				*(size_t *)&Random->Term[V].Link[E].Term = (Random_Term[(Minimum + E) % Random->Term[V].Links] + V + 1) % Random->Terms;
				*(double *)&Random->Term[V].Link[E].Real = 1;
			}
		}
	}
	return Random;
}
