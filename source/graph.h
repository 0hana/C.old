/* Hanami Zero (C) 2021: graph.h */
#ifndef _graph_
#define _graph_

#include "0hana/memory.h"

typedef struct graph_adjacency {
	x _c Vertex;
	x  _c Edges;
	r * _c Edge;
} graph_adjacency;

typedef struct graph_vertex {
	x  _c Adjacencyes;
	graph_adjacency * _c Adjacency;
} graph_vertex;

typedef struct graph {
	x  _c Vertexes;
	graph_vertex * _c Vertex;
} graph;

v  free_graph(graph * c G);
graph * graph_transpose(graph c * c G);

#endif//_graph_
