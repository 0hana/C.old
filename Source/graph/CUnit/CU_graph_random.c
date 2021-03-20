//Zero Hanami (C) 2021: CU_graph_random.c

#include <stdio.h>
#include "CU_graph.h"
#include "0hana/random/random.h"
#define VERTEX_LIMIT 100
#define EDGE_LIMIT -1

void CU_graph_random(void) {

	unsigned int const Failures = CU_get_number_of_failures();

	for(size_t I = 0; I < 1000; I++) {

		struct graph * const G = graph_random(VERTEX_LIMIT, EDGE_LIMIT);
		CU_ASSERT_PTR_NOT_NULL_FATAL(G);
		CU_ASSERT(G->Terms < VERTEX_LIMIT);

		for(size_t V = 0; V < G->Terms; V++) {
			CU_ASSERT(G->Term[V].Links < G->Terms);

			if(G->Term[V].Links > 0) CU_ASSERT(G->Term[V].Link[0].Term < G->Terms);
			for(size_t E = 1; E < G->Term[V].Links; E++) {
				CU_ASSERT(G->Term[V].Link[E].Term < G->Terms);
				CU_ASSERT(G->Term[V].Link[E].Term > G->Term[V].Link[E - 1].Term);
			}
		}
		
		graph_free(G);

		CU_ASSERT_EQUAL_FATAL(Failures, CU_get_number_of_failures());
	}
}
