//Zero Hanami (C) 2021: graph_node_allocate.c

#include "0hana/graph.h"

struct graph_node * graph_node_allocate(size_t const Nodes) {
	if(!Nodes) return NULL;

	struct graph_node * Node = (struct graph_node *)malloc(sizeof(struct graph_node) * Nodes);
	if(!Node) return NULL;

	for(size_t N = 0; N < Nodes; N++) {
		_relax(Node[N].Links, size_t) = 0;
		_relax(Node[N].Link, struct graph_link *) = NULL;
	}

	return Node;
}
