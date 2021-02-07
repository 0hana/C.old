//Zero Hanami (C) 2021
#ifndef _GRAPH
#define _GRAPH

#include <stdlib.h>

struct graph {
	size_t const Vertices;
	size_t const * const Adjacencies;//(graph)G.Adjacencies[0]: the number adjacent vertices to the 0th
	size_t const * const * const Edge;//(graph)G.Edge[u][v]: the edge from vertex u to vertex v
};

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

#endif//_GRAPH
