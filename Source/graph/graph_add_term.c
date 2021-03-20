//Zero Hanami (C) 2021: graph_add_term.c

#include "graph.h"

struct graph * graph_add_term(struct graph * const G) {
	if(!G) return NULL;

	struct graph_term * Copy = G->Term;

	if(!(*(struct graph_term **)&G->Term = (struct graph_term *)realloc(G->Term, sizeof(struct graph_term) * (G->Terms + 1))))
		return *(struct graph_term **)&G->Term = Copy, NULL;

	return *(size_t *)&G->Terms += 1, G;
}
