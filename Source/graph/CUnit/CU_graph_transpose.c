//Zero Hanami (C) 2021: CU_graph_transpose.c

#include <stdio.h>
#include "CU_graph.h"
#include "0hana/random/random.h"
#define VERTEX_LIMIT 10
#define EDGE_LIMIT -1

void CU_graph_transpose(void) {

	unsigned int const Failures = CU_get_number_of_failures();

	for(size_t I = 0; I < 100; I++) {

		struct graph * const G = graph_random(VERTEX_LIMIT, EDGE_LIMIT);
		CU_ASSERT_PTR_NOT_NULL_FATAL(G);

		struct graph * const T = graph_transpose(G);
		if(!T) {
			graph_free(G);
			CU_FAIL_FATAL("Failed to allocate Transpose graph -- stopping test");
		}

		CU_ASSERT_EQUAL(T->Terms, G->Terms);

		//Test that G edge destinations as T edge sources point to the vertex of that edge in G
		size_t T_Term_Link_Iterator[G->Terms];
		for(size_t V = 0; V < G->Terms; V++) T_Term_Link_Iterator[V] = 0;
		for(size_t V = 0; V < G->Terms; V++) {
			for(size_t E = 0; E < G->Term[V].Links; E++) {
				#define G_DEST G->Term[V].Link[E].Term
				CU_ASSERT_EQUAL(V, T->Term[G_DEST].Link[T_Term_Link_Iterator[G_DEST]++].Term);
				#undef G_DEST
			}
		}

		graph_free(T);
		graph_free(G);

		//Problem?:
		CU_ASSERT_EQUAL_FATAL(Failures, CU_get_number_of_failures());
	}
}
