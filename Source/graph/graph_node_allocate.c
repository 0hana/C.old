//Zero Hanami (C) 2021: graph_node_allocate.c

#include "0hana/graph.h"

struct graph_node * graph_node_allocate(size_t const Nodes) {
	return (struct graph_node *)malloc(sizeof(struct graph_node) * Nodes);
}
