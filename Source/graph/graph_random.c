//Zero Hanami (C) 2021: graph_random.c

#include "0hana/graph.h"

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
				*(double *)&Random->Vertex[V].Edge[E].Weight = 1;
			}
		}
	}
	return Random;
}
