//Zero Hanami (C) 2021: graph_copy.c

#include "graph.h"

struct graph * graph_copy(struct graph const * const G) {
	struct graph * Copy = malloc(sizeof(struct graph));
	if(!Copy) return NULL;
	*(size_t *)&Copy->Vertices = G->Vertices;
	*(struct graph_vertex **)&G->Vertex = malloc(sizeof(struct graph_vertex) * G->Vertices);
	if(!Copy->Vertex) return free(Copy), NULL;
	for(size_t V = 0; V < G->Vertices; V++) {
		*(struct graph_vertex_edge **)&Copy->Vertex[V].Edge = malloc(sizeof(struct graph_vertex_edge) * G->Vertex[V].Edges);
		if(!Copy->Vertex[V].Edge) {
			while(V-- > 0) free(Copy->Vertex[V].Edge);
			return free(Copy->Vertex), free(Copy), NULL;
		}
		*(size_t *)&Copy->Vertex[V].Edges = G->Vertex[V].Edges;
		for(size_t E = 0; E < G->Vertex[V].Edges; E++) {
			*(size_t *)&Copy->Vertex[V].Edge[E].Destination = G->Vertex[V].Edge[E].Destination;
			*(double *)&Copy->Vertex[V].Edge[E].Weight = G->Vertex[V].Edge[E].Weight;
		}
	}
	return Copy;
}

void graph_free(struct graph * const G) {
	for(size_t V = 0; V < G->Vertices; V++) free(G->Vertex[V].Edge);
	free(G->Vertex);
	free(G);
}
