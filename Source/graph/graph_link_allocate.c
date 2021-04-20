//Zero Hanami (C) 2021: graph_link_allocate.c

#include "0hana/graph.h"

struct graph_link * graph_link_allocate(size_t const Links) {
	if(!Links) return NULL;

	struct graph_link * Link = (struct graph_link *)malloc(sizeof(struct graph_link) * Links);
	if(!Link) return NULL;

	for(size_t L = 0; L < Links; L++) {
		_relax(Link[L].Term, size_t)   =   -1;
		_relax(Link[L].Reals, size_t)  =    0;
		_relax(Link[L].Real, double *) = NULL;
	}

	return Link;
}
