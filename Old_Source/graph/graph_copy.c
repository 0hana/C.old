//Zero Hanami (C) 2021: graph_copy.c

#include "graph.h"

struct graph * graph_copy(struct graph const * const G) {
	struct graph * Copy = (struct graph *)malloc(sizeof(struct graph));
	if(!Copy) return NULL;
	*(size_t *)&Copy->Terms = G->Terms;
	*(struct graph_term **)&G->Term = (struct graph_term *)malloc(sizeof(struct graph_term) * G->Terms);
	if(!Copy->Term) return free(Copy), NULL;
	for(size_t V = 0; V < G->Terms; V++) {
		*(struct graph_term_link **)&Copy->Term[V].Link = (struct graph_term_link *)malloc(sizeof(struct graph_term_link) * G->Term[V].Links);
		if(!Copy->Term[V].Link) {
			while(V-- > 0) free(Copy->Term[V].Link);
			return free(Copy->Term), free(Copy), NULL;
		}
		*(size_t *)&Copy->Term[V].Links = G->Term[V].Links;
		for(size_t E = 0; E < G->Term[V].Links; E++) {
			*(size_t *)&Copy->Term[V].Link[E].Term = G->Term[V].Link[E].Term;
			*(double *)&Copy->Term[V].Link[E].Real = G->Term[V].Link[E].Real;
		}
	}
	return Copy;
}

void graph_free(struct graph * const G) {
	for(size_t V = 0; V < G->Terms; V++) free(G->Term[V].Link);
	free(G->Term);
	free(G);
}
