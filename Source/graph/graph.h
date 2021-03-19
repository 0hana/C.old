//Zero Hanami (C) 2021: graph.h

#ifndef _GRAPH
#define _GRAPH
#include <stdlib.h>

struct graph_term_link {
	size_t const Term;
	double const Real;
};
struct graph_term {
	size_t const Links;
	struct graph_term_link * const Link;
};
struct graph {
	size_t const Terms;
	struct graph_term * const Term;
} * graph_copy(struct graph const * const G)
, * graph_transpose(struct graph const * const G)
, * graph_random(size_t const Term_Limit, size_t const Link_Limit);

void graph_free(struct graph * const G), graph_print(struct graph const * const G);


struct graph_bfs_data {
	size_t const * const Distance;
	size_t const * const Parent;
} * graph_bfs(struct graph const * const G, size_t const Source);

void graph_bfs_data_free(struct graph_bfs_data * const D);


struct graph_dfs_data {
	enum boolean { true = ~0, false = 0 } const Cyclic;
	size_t const * const Topological_Order;
} * graph_dfs(struct graph const * const G, size_t const Source);

void graph_dfs_data_free(struct graph_dfs_data * const D);

#endif//_GRAPH
