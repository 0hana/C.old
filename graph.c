//Zero Hanami (C) 2021

#include "graph.h"
#include "queue.h"

struct graph_bfs_data const * graph_bfs(struct graph * const G, size_t Source) {
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
	if(!queue_push(Vertex_Queue, Source)) return free(Distance), free(Parent), free(Data), NULL;
	while(Vertex_Queue.Tail) {
		size_t Vertex = queue_pop(Vertex_Queue);
		for(size_t Adjacency = 0; Adjacency < G->Adjacencies[Vertex]; Adjacency++) {
			size_t const Neighbor = G->Edge[Vertex][Adjacency];
			if(Distance[Neighbor] == -1) {
				Distance[Neighbor] = Distance[Vertex] + 1;
				Parent[Neighbor] = Vertex;
				if(!queue_push(Vertex_Queue, Neighbor)) return free(Distance), free(Parent), free(Data), NULL;
			}
		}
	}
	return Data;
}

struct graph_dfs_data const * graph_dfs(struct graph * const G, size_t Source) {
}
