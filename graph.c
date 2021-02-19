//Zero Hanami (C) 2021: graph.c

#include "graph.h"
#include "queue.h"

struct graph * graph_transpose(struct graph const * const G) {
	//Allocate return structures
	struct graph * Transpose = malloc(sizeof(struct graph));
	if(!Transpose) return NULL;
	*(struct graph_vertex **)&Transpose->Vertex = malloc(sizeof(struct graph_vertex) * G->Vertices);
	if(!Transpose->Vertex) return free(Transpose), NULL;
	else {
		//Determine number of edges per transpose vertex
		for(size_t V = 0; V < G->Vertices; V++) *(size_t *)&Transpose->Vertex[V].Edges = 0;
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
			for(size_t E = 0; E < G->Vertex[V].Edges; E++) {
				#define X G->Vertex[V].Edge[E]
				*(size_t *)&Transpose->Vertex[X.Destination].Edge[Adjacent[X.Destination]].Destination = V;
				Transpose->Vertex[X.Destination].Edge[Adjacent[X.Destination]++].Weight = X.Weight;
				#undef X
			}
		}
	}
	return Transpose;
}


struct graph * graph_random(size_t const Vertex_Limit, size_t const Edge_Limit) {
	//Allocate return structures
	struct graph * Random = malloc(sizeof(struct graph));
	if(!Random) return NULL;
	*(struct graph_vertex **)&Random->Vertex = malloc(sizeof(struct graph_vertex) * (*(size_t *)&Random->Vertices = (unsigned)rand() % Vertex_Limit));
	if(!Random->Vertex) return free(Random), NULL;
	else {
		
		//Determine number of edges per random vertex and Allocate edge array for each random vertex
		
		for(size_t V = 0; V < Random->Vertices; V++) {
			*(struct graph_vertex_edge **)&Random->Vertex[V].Edge = malloc(sizeof(struct graph_vertex_edge) * (*(size_t *)&Random->Vertex[V].Edges = (unsigned)rand() % Random->Vertices % Edge_Limit));
			if(!Random->Vertex[V].Edge) {
				//Release all allocations prior to malloc failure
				while(V-- > 0) free(Random->Vertex[V].Edge);
				return free(Random->Vertex), free(Random), NULL;
			}
			//Generate the random edge destinations in ascending order
			size_t Random_Destination[Random->Vertex[V].Edges], Minimum = 0;

			random_array_of_unique_indices(Random_Destination, Random->Vertices - 1, Random->Vertex[V].Edges);
			//function will only fail on insufficient stack memory -- requires Random->Vertex[V].Edges * sizeof(size_t) + constant to run successfully

			for(size_t E = 0; E < Random->Vertex[V].Edges; E++) if((Random_Destination[E] + V + 1) % Random->Vertices < (Random_Destination[Minimum] + V + 1) % Random->Vertices) Minimum = E;
			for(size_t E = 0; E < Random->Vertex[V].Edges; E++) {
				*(size_t *)&Random->Vertex[V].Edge[E].Destination = (Random_Destination[(Minimum + E) % Random->Vertex[V].Edges] + V + 1) % Random->Vertices;
				Random->Vertex[V].Edge[E].Weight = 1;
			}
		}
	}
	return Random;
}


void graph_free(struct graph * const G) {
	for(size_t V = 0; V < G->Vertices; V++) free(G->Vertex[V].Edge);
	free(G->Vertex);
	free(G);
}


void graph_print(struct graph const * const G) {
	printf("Vertices: %lu\n", G->Vertices);
	for(size_t V = 0; V < G->Vertices; V++) {
		printf("%lu :", V);
		for(size_t E = 0; E < G->Vertex[V].Edges; E++) {
			printf("\t%lu\t%f\n", G->Vertex[V].Edge[E].Destination, G->Vertex[V].Edge[E].Weight);
		}
		printf("\n");
	}
}


struct graph_bfs_data * graph_bfs(struct graph const * const G, size_t const Source) {
	//Allocate return structures
	struct graph_bfs_data * const Data = malloc(sizeof(struct graph_bfs_data));
	if(!Data) return NULL;
	*(size_t **)&Data->Distance = malloc(sizeof(size_t) * G->Vertices);
	if(!Data->Distance) return free(Data), NULL;
	*(size_t **)&Data->Parent = malloc(sizeof(size_t) * G->Vertices);
	if(!Data->Parent) return free(*(size_t **)&Data->Distance), free(Data), NULL;

	for(size_t V = 0; V < G->Vertices; V++) {
		*(size_t *)&Data->Distance[V] = -1;
		*(size_t *)&Data->Parent[V] = V;
	}

	//Initiate BFS
	*(size_t *)&Data->Distance[Source] = 0;
	struct queue Vertex_Queue = { .Head = NULL, .Tail = NULL };
	if(!queue_push(&Vertex_Queue, Source)) return free(*(size_t **)&Data->Parent), free(*(size_t **)&Data->Distance), free(Data), NULL;
	while(Vertex_Queue.Head) {
		size_t V = queue_pop(&Vertex_Queue);
		for(size_t E = 0; E < G->Vertex[V].Edges; E++) {
			#define Neighbor G->Vertex[V].Edge[E].Destination
			if(Data->Distance[Neighbor] == (size_t)-1) {
				*(size_t *)&Data->Distance[Neighbor] = Data->Distance[V] + 1;
				*(size_t *)&Data->Parent[Neighbor] = V;
				if(!queue_push(&Vertex_Queue, Neighbor)) return free(*(size_t **)&Data->Parent), free(*(size_t **)&Data->Distance), free(Data), NULL;
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

/*
struct graph_dfs_data const * graph_dfs(struct graph * const G, size_t Source) {
}
*/
