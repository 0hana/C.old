//Zero Hanami (C) 2021: graph_translate_nodes.c

#include <omp.h>
#include "0hana/graph.h"

bool graph_translate_nodes(struct graph * const G, size_t const * const Translation_Array) {
	#define TA Translation_Array
	#define NA Node_Array
	struct graph_node * const NA = (struct graph_node *)malloc(sizeof(struct graph_node) * G->Nodes);
	if(!NA) return false;//insufficient memory to run the algorithm
	#pragma omp parallel for
	for(size_t N = 0; N < G->Nodes; N++) {
		_relax(NA[N].Links, size_t) = G->Node[N].Links;
		_relax(NA[N].Link, struct graph_link *) = G->Node[N].Link;
	}
	#pragma omp parallel for
	for(size_t N = 0; N < G->Nodes; N++) {
		_relax(G->Node[TA[N]].Links, size_t) = NA[N].Links;
		_relax(G->Node[TA[N]].Link, struct graph_link *) = NA[N].Link;
	}
	#pragma omp parallel for
	for(size_t N = 0; N < G->Nodes; N++)
		#pragma omp parallel for
		for(size_t L = 0; L < G->Node[N].Links; L++) _relax(NA[N].Link[L].Term, size_t) = TA[NA[N].Link[L].Term];
	free(NA);
	return true;
	#undef NA
	#undef TA
}

//Using a represention of graphs where parallel edges exist in their own array grouped under a single vertex terminal, vertex translation can be achieved in O((V^2)/P) time and with O(V) memory, where V is the number of vertices in the graph and P is the number of parallel compute units available to operate on the graph at once
