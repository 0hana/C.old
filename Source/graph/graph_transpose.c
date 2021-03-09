//Zero Hanami (C) 2021: graph_transpose.c

#include "graph.h"

struct graph * graph_transpose(struct graph const * const G) {
	//Allocate return structures
	struct graph * Transpose = (struct graph *)malloc(sizeof(struct graph));
	if(!Transpose) return NULL;
	*(struct graph_vertex **)&Transpose->Vertex = (struct graph_vertex *)malloc(sizeof(struct graph_vertex) * G->Vertices);
	if(!Transpose->Vertex) return free(Transpose), NULL;
	else {
		//Determine number of edges per transpose vertex
		for(size_t V = 0; V < G->Vertices; V++) *(size_t *)&Transpose->Vertex[V].Edges = 0;
		for(size_t V = 0; V < G->Vertices; V++) for(size_t E = 0; E < G->Vertex[V].Edges; E++) (*(size_t *)&Transpose->Vertex[G->Vertex[V].Edge[E].Destination].Edges)++;
		//Allocate edge array for each transpose vertex
		for(size_t V = 0; V < G->Vertices; V++) {
			*(struct graph_vertex_edge **)&Transpose->Vertex[V].Edge = (struct graph_vertex_edge *)malloc(sizeof(struct graph_vertex_edge) * Transpose->Vertex[V].Edges);
			if(!Transpose->Vertex[V].Edge) {
				//Release all allocations prior to malloc failure
				while(V-- > 0) free(Transpose->Vertex[V].Edge);
				return free(Transpose->Vertex), free(Transpose), NULL;
			}
		}
	}

	//Translate G to Transpose
	*(size_t *)&Transpose->Vertices = G->Vertices;
	{
		size_t Adjacent[G->Vertices];//Counter for each transpose vertex
		for(size_t V = 0; V < G->Vertices; V++) Adjacent[V] = 0;
		for(size_t V = 0; V < G->Vertices; V++) {
			for(size_t E = 0; E < G->Vertex[V].Edges; E++) {
				#define X G->Vertex[V].Edge[E]
				*(size_t *)&Transpose->Vertex[X.Destination].Edge[Adjacent[X.Destination]].Destination = V;
				*(double *)&Transpose->Vertex[X.Destination].Edge[Adjacent[X.Destination]++].Weight = X.Weight;
				#undef X
			}
		}
	}
	return Transpose;
}
