//Zero Hanami (C) 2021: graph_dfs.c

#include "graph.h"

enum color { black, grey, white };
struct time_stamps { size_t Start, Finish; };

void graph_dfs_visit(struct graph const * const G, size_t const Current_Term, size_t * const Time, enum color * const Color, struct time_stamps * const Time_Stamps, size_t * const Topological_Iterator, struct graph_dfs_data * const Data) {
	Color[Current_Term] = grey;
	Time_Stamps[Current_Term].Start = (*Time)++;

	for(size_t E = 0; E < G->Term[Current_Term].Links; E++) {
		switch(Color[G->Term[Current_Term].Link[E].Term]) {
			case white: graph_dfs_visit(G, G->Term[Current_Term].Link[E].Term, Time, Color, Time_Stamps, Topological_Iterator, Data);
				break;
			case grey: *(enum boolean *)&Data->Cyclic = true;
				break;
			case black:
				break;
			default:
				break;
		}
	}

	Color[Current_Term] = black;
	Time_Stamps[Current_Term].Finish = (*Time)++;
	*(size_t *)&Data->Topological_Order[G->Terms - ++(*Topological_Iterator)] = Current_Term;
}

struct graph_dfs_data * graph_dfs(struct graph const * const G, size_t const Source) {
	struct graph_dfs_data * Data = (struct graph_dfs_data *)malloc(sizeof(struct graph_dfs_data));
	if(!Data) return NULL;
	*(size_t **)&Data->Topological_Order = (size_t *)malloc(sizeof(size_t) * G->Terms);
	if(!Data->Topological_Order) return free(Data), NULL;
	*(enum boolean *)&Data->Cyclic = false;

	enum color Color[G->Terms];
	struct time_stamps Time_Stamps[G->Terms];
	for(size_t V = 0; V < G->Terms; V++) {
		Color[V] = white;
		Time_Stamps[V].Start = 0;
		Time_Stamps[V].Finish = 0;
	}
	size_t Time = 0, Topological_Iterator = 0;


	for(size_t V = 0; V < G->Terms; V++) {
		if(Color[(Source + V) % G->Terms] == white) {
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
