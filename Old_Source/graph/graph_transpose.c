//Zero Hanami (C) 2021: graph_transpose.c

#include "graph.h"

struct graph * graph_transpose(struct graph const * const G) {
	//Allocate return structures
	struct graph * Transpose = (struct graph *)malloc(sizeof(struct graph));
	if(!Transpose) return NULL;
	*(struct graph_term **)&Transpose->Term = (struct graph_term *)malloc(sizeof(struct graph_term) * G->Terms);
	if(!Transpose->Term) return free(Transpose), NULL;
	else {
		//Determine number of edges per transpose vertex
		for(size_t V = 0; V < G->Terms; V++) *(size_t *)&Transpose->Term[V].Links = 0;
		for(size_t V = 0; V < G->Terms; V++) for(size_t E = 0; E < G->Term[V].Links; E++) (*(size_t *)&Transpose->Term[G->Term[V].Link[E].Term].Links)++;
		//Allocate edge array for each transpose vertex
		for(size_t V = 0; V < G->Terms; V++) {
			*(struct graph_term_link **)&Transpose->Term[V].Link = (struct graph_term_link *)malloc(sizeof(struct graph_term_link) * Transpose->Term[V].Links);
			if(!Transpose->Term[V].Link) {
				//Release all allocations prior to malloc failure
				while(V-- > 0) free(Transpose->Term[V].Link);
				return free(Transpose->Term), free(Transpose), NULL;
			}
		}
	}

	//Translate G to Transpose
	*(size_t *)&Transpose->Terms = G->Terms;
	{
		size_t Adjacent[G->Terms];//Counter for each transpose vertex
		for(size_t V = 0; V < G->Terms; V++) Adjacent[V] = 0;
		for(size_t V = 0; V < G->Terms; V++) {
			for(size_t E = 0; E < G->Term[V].Links; E++) {
				#define X G->Term[V].Link[E]
				*(size_t *)&Transpose->Term[X.Term].Link[Adjacent[X.Term]].Term = V;
				*(double *)&Transpose->Term[X.Term].Link[Adjacent[X.Term]++].Real = X.Real;
				#undef X
			}
		}
	}
	return Transpose;
}
