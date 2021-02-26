//Zero Hanami (C) 2021: graph.h

#ifndef _GRAPH
#define _GRAPH
#include <stdlib.h>

struct graph {
	size_t const Vertices;
	struct graph_vertex {
		size_t const Edges;
		struct graph_vertex_edge {
			size_t const Destination;
			double const Weight;
		} * const Edge;
	} * const Vertex;
} * graph_copy(struct graph const * const G)
, * graph_transpose(struct graph const * const G)
, * graph_random(size_t const Vertex_Limit, size_t const Edge_Limit);

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
