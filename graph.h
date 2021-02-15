//Zero Hanami (C) 2021: graph.h

#ifndef _GRAPH
#define _GRAPH
#include <stdio.h>
#include "random.h"

struct graph {
	size_t const Vertices;
	struct graph_vertex {
		size_t const Edges;
		struct graph_vertex_edge {
			size_t const Destination;
			float Weight;
		} * const Edge;
	} * const Vertex;
} * graph_transpose(struct graph const * const G)
, * graph_random(size_t const Vertex_Limit, size_t const Edge_Limit);

void graph_free(struct graph * const G), graph_print(struct graph const * const G);

/*
struct graph_bfs_data {
	size_t const * const Distance;
	size_t const * const Parent;
} const * graph_bfs(struct graph * const G, size_t Source);

struct graph_dfs_data {
	struct time_stamps {
		size_t const start;
		size_t const finish;
	} const * const Time_Stamps;
	enum edge_type { tree_edge, back_edge, forward_edge } const * const * const Edge_Type;
	enum boolean { true = ~0, false = 0 } const Cyclic;
	size_t const Topological_Order[];
} const * graph_dfs(struct graph * const G, size_t Source);
*/

#endif//_GRAPH
