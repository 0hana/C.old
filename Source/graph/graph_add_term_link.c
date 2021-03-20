//Zero Hanami (C) 2021: graph_add_term.c

#include "graph.h"

struct graph * graph_add_term_link(struct graph * const G, size_t const Root, size_t const Term, double const Real) {
	//The Root is the Term the Link emerges from
	if(!G || G->Terms <= Root) return NULL;

	struct graph_term_link * Copy = G->Term[Root].Link;

	if(!(*(struct graph_term_link **)&G->Term[Root].Link = (struct graph_term_link *)realloc(G->Term, sizeof(struct graph_term_link) * (G->Term[Root].Links + 1))))
		return *(struct graph_term_link **)&G->Term[Root].Link = Copy, NULL;

	return
		*(size_t *)&G->Term[Root].Link[G->Term[Root].Links].Term = Term,
		*(size_t *)&G->Term[Root].Link[G->Term[Root].Links].Real = Real,
		*(size_t *)&G->Term[Root].Links += 1,
		G;
}
