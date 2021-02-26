//Zero Hanami (C) 2021: graph_dfs.c

#include "graph.h"

enum color { black, grey, white };
struct time_stamps { size_t Start, Finish; };

void graph_dfs_visit(struct graph const * const G, size_t const Current_Vertex, size_t * const Time, enum color * const Color, struct time_stamps * const Time_Stamps, size_t * const Topological_Iterator, struct graph_dfs_data * const Data) {
	Color[Current_Vertex] = grey;
	Time_Stamps[Current_Vertex].Start = (*Time)++;

	for(size_t E = 0; E < G->Vertex[Current_Vertex].Edges; E++) {
		switch(Color[G->Vertex[Current_Vertex].Edge[E].Destination]) {
			case white: graph_dfs_visit(G, G->Vertex[Current_Vertex].Edge[E].Destination, Time, Color, Time_Stamps, Topological_Iterator, Data);
				break;
			case grey: *(enum boolean *)&Data->Cyclic = true;
				break;
			case black:
				break;
			default:
				break;
		}
	}

	Color[Current_Vertex] = black;
	Time_Stamps[Current_Vertex].Finish = (*Time)++;
	*(size_t *)&Data->Topological_Order[G->Vertices - ++(*Topological_Iterator)] = Current_Vertex;
}

struct graph_dfs_data * graph_dfs(struct graph const * const G, size_t const Source) {
	struct graph_dfs_data * Data = malloc(sizeof(struct graph_dfs_data));
	if(!Data) return NULL;
	*(size_t **)&Data->Topological_Order = malloc(sizeof(size_t) * G->Vertices);
	if(!Data->Topological_Order) return free(Data), NULL;
	*(enum boolean *)&Data->Cyclic = false;

	enum color Color[G->Vertices];
	struct time_stamps Time_Stamps[G->Vertices];
	for(size_t V = 0; V < G->Vertices; V++) {
		Color[V] = white;
		Time_Stamps[V].Start = 0;
		Time_Stamps[V].Finish = 0;
	}
	size_t Time = 0, Topological_Iterator = 0;


	for(size_t V = 0; V < G->Vertices; V++) {
		if(Color[(Source + V) % G->Vertices] == white) {
			graph_dfs_visit(G, V, &Time, Color, Time_Stamps, &Topological_Iterator, Data);
		}
	}
	return Data;
}

void graph_dfs_data_free(struct graph_dfs_data * const D) {
	free((enum boolean *)&D->Cyclic);
	free((size_t *)D->Topological_Order);
	free(D);
}
