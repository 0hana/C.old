//Zero Hanami (C) 2021
#ifndef _GRAPH
#define _GRAPH

#include <stdlib.h>

struct graph {
	size_t const Vertices;
	size_t const * const Adjacencies;
	struct graph_edge {
		float Weight;
		size_t Destination;
	} const * const * const Edge;
} * graph_transpose(struct graph * G) {
	//Allocate return structures
	struct graph * Transpose = malloc(sizeof(struct graph));
	if(!Transpose) return NULL;
	size_t * Adjacencies = malloc(sizeof(size_t) * G->Vertices);
	if(!Adjacencies) return free(Transpose), NULL;
	else {
		//Determine Adjacencies for each vertex in Transpose;
		for(size_t V = 0; V < G->Vertices; V++) Adjacencies[V] = 0;
		for(size_t V = 0; V < G->Vertices; V++) {
			for(size_t Adjacent = 0; Adjacent < G->Adjacencies[V]; Adjacent++) {
				Adjacencies[G->Edge[V][Adjacent].Destination]++;
			}
		}
	}
	struct graph_edge ** Edge = malloc(sizeof(struct graph_edge *) * G->Vertices);
	if(!Edge) return free(Adjacencies), free(Transpose), NULL;
	else {
		for(size_t V = 0; V < G->Vertices; V++) {
			Edge[V] = malloc(sizeof(struct graph_edge) * Adjacencies[V]);
			if(!Edge[V]) {
				while(V-- != (size_t)-1) free(Edge[V]);
				return free(Edge), free(Adjacencies), free(Transpose), NULL;
			}
		}
	}

	//Initialize return structures
	*(size_t *)&Transpose->Vertices = G->Vertices;
	*(size_t **)&Transpose->Adjacencies = Adjacencies;
	*(struct graph_edge ***)&Transpose->Edge = Edge;
	{
		size_t Adjacency[G->Vertices];
		for(size_t V = 0; V < G->Vertices; V++) Adjacency[V] = 0;
		for(size_t V = 0; V < G->Vertices; V++) {
			for(size_t Adjacent = 0; Adjacent < G->Adjacencies[V]; Adjacent++) {
				*(struct graph_edge *)&Transpose->Edge[G->Edge[V][Adjacent].Destination][Adjacency[G->Edge[V][Adjacent].Destination]++] = (struct graph_edge){ .Destination = V, .Weight = G->Edge[V][Adjacent].Weight };
			}
		}
	}
	return Transpose;	
}

void graph_free(struct graph * const G) {
	for(size_t V = 0; V < G->Vertices; V++) free(G->Edge[V]);
	free(G->Adjacencies);
	free(G->Edge);
	free(G);
}

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
