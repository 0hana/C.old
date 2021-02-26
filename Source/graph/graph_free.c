//Zero Hanami (C) 2021: graph_free.c

#include "0hana/graph.h"

void graph_free(struct graph * const G) {
	for(size_t V = 0; V < G->Vertices; V++) free(G->Vertex[V].Edge);
	free(G->Vertex);
	free(G);
}
