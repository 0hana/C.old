//Zero Hanami (C) 2021: graph_bfs.c

#include "graph.h"
#include "../queue/queue.h"

struct graph_bfs_data * graph_bfs(struct graph const * const G, size_t const Source) {
	//Allocate return structures
	struct graph_bfs_data * const Data = (struct graph_bfs_data *)malloc(sizeof(struct graph_bfs_data));
	if(!Data) return NULL;
	*(size_t **)&Data->Distance = (size_t *)malloc(sizeof(size_t) * G->Vertices);
	if(!Data->Distance) return free(Data), NULL;
	*(size_t **)&Data->Parent = (size_t *)malloc(sizeof(size_t) * G->Vertices);
	if(!Data->Parent) return free(*(size_t **)&Data->Distance), free(Data), NULL;

	for(size_t V = 0; V < G->Vertices; V++) {
		*(size_t *)&Data->Distance[V] = (size_t)-1;
		*(size_t *)&Data->Parent[V] = V;
	}

	//Initiate BFS
	*(size_t *)&Data->Distance[Source] = 0;
	struct queue Vertex_Queue = { .Head = NULL, .Tail = NULL };
	if(!queue_push(&Vertex_Queue, Source)) return free(*(size_t **)&Data->Parent), free(*(size_t **)&Data->Distance), free(Data), NULL;
	while(Vertex_Queue.Head) {
		size_t const V = queue_pop(&Vertex_Queue);
		for(size_t E = 0; E < G->Vertex[V].Edges; E++) {
			#define Neighbor G->Vertex[V].Edge[E].Destination
			if(Data->Distance[Neighbor] == (size_t)-1) {
				*(size_t *)&Data->Distance[Neighbor] = Data->Distance[V] + 1;
				*(size_t *)&Data->Parent[Neighbor] = V;
				if(!queue_push(&Vertex_Queue, Neighbor)) return graph_bfs_data_free(Data), NULL;
				#undef Neighbor
			}
		}
	}
	return Data;
}

void graph_bfs_data_free(struct graph_bfs_data * const D) {
	free(*(size_t **)&D->Distance);
	free(*(size_t **)&D->Parent);
	free(D);
}
