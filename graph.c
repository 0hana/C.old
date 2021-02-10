//Zero Hanami (C) 2021: graph.c

#include "graph.h"
/* graph construct

struct graph {
	size_t const Vertices;
	struct graph_vertex {
		size_t const Edges;
		struct graph_vertex_edge {
			size_t const Destination;
			float Weight;
		} * const Edge;
	} * const Vertex;
};

*/

struct graph * graph_transpose(struct graph const * const G) {
	//Allocate return structures
	struct graph * Transpose = malloc(sizeof(struct graph));
	if(!Transpose) return NULL;
	*(struct graph_vertex **)&Transpose->Vertex = malloc(sizeof(struct graph_vertex) * G->Vertices);
	if(!Transpose->Vertex) return free(Transpose), NULL;
	else {
		//Determine number of edges per transpose vertex
		for(size_t V = 0; V < G->Vertices; V++) *(size_t *)Transpose->Vertex[V].Edges = 0;
		for(size_t V = 0; V < G->Vertices; V++) for(size_t E = 0; E < G->Vertex[V].Edges; E++) (*(size_t *)&Transpose->Vertex[G->Vertex[V].Edge[E].Destination].Edges)++;
		//Allocate edge array for each transpose vertex
		for(size_t V = 0; V < G->Vertices; V++) {
			*(struct graph_vertex_edge **)&Transpose->Vertex[V].Edge = malloc(sizeof(struct graph_vertex_edge) * Transpose->Vertex[V].Edges);
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
			for(size_t E = 0; V < G->Vertex[V].Edges; E++) {
				#define X G->Vertex[V].Edge[E]
				*(size_t *)&Transpose->Vertex[X.Destination].Edge[Adjacent[X.Destination]].Destination = V;
				Transpose->Vertex[X.Destination].Edge[Adjacent[X.Destination]++].Weight = X.Weight;
				#undef X
			}
		}
	}
	return Transpose;
}

void graph_free(struct graph * const G) {
	for(size_t V = 0; V < G->Vertices; V++) free(G->Vertex[V].Edge);
	free(G->Vertex);
	free(G);
}

/*
//Zero Hanami (C) 2021

#include "graph.h"
#include "queue.h"

struct graph_bfs_data const * graph_bfs(struct graph * const G, size_t const Source) {
	//Allocate return structures
	size_t * Distance = malloc(G->Vertices*sizeof(size_t));
	if(!Distance) return NULL;
	size_t * Parent  = malloc(G->Vertices*sizeof(size_t));
	if(!Parent) return free(Distance), NULL;
	struct graph_bfs_data * const Data = malloc(sizeof(struct graph_bfs_data));
	if(!Data) return free(Distance), free(Parent), NULL;

	//Initialize return structures
	*(size_t **)&Data->Distance = Distance;
	*(size_t **)&Data->Parent = Parent;

	for(size_t Vertex = 0; Vertex < G->Vertices; Vertex++) {
		Distance[Vertex] = -1;
		Parent[Vertex] = Vertex;
	}

	//Initiate BFS
	Distance[Source] = 0;
	struct queue Vertex_Queue = { .Head = NULL, .Tail = NULL, .Unit = sizeof(size_t) };
	if(!queue_push(Vertex_Queue, &Source)) return free(Distance), free(Parent), free(Data), NULL;
	while(Vertex_Queue.Tail) {
		size_t Vertex;
		{
			size_t * Address = (size_t *)queue_pop(Vertex_Queue)
		}
		for(size_t Adjacency = 0; Adjacency < G->Adjacencies[Vertex]; Adjacency++) {
			#define Neighbor G->Edge[Vertex][Adjacency]
			if(Distance[Neighbor] == -1) {
				Distance[Neighbor] = Distance[Vertex] + 1;
				Parent[Neighbor] = Vertex;
				if(!queue_push(Vertex_Queue, &Neighbor)) return free(Distance), free(Parent), free(Data), NULL;
				#undef Neighbor
			}
		}
	}
	return Data;
}

struct graph_dfs_data const * graph_dfs(struct graph * const G, size_t Source) {
}
*/