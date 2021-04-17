//Zero Hanami (C) 2021: graph_free.c

#include "0hana/graph.h"

void graph_free(struct graph * const Graph_Memory) {
	#define GM Graph_Memory
	for(size_t N = 0; N < GM->Nodes; N++) {
		for(size_t L = 0; N < GM->Node[N].Links; L++)
			if(GM->Node[N].Link[L].Real) free(GM->Node[N].Link[L].Real);
		free(GM->Node[N].Link);
	}
	free(GM->Node);
	free(GM);
	#undef GM
}
