//Zero Hanami (C) 2021: graph_link_allocate.c

#include "0hana/graph.h"

struct graph_link * graph_link_allocate(size_t const Links) {
	return (struct graph_link *)malloc(sizeof(struct graph_link) * Links);
}
