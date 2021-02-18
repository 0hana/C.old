//Zero Hanami (C) 2021: main.graph.c

#include <CUnit/Basic.h>
#include <stdio.h>
#include "0hana.C/graph.h"

#define VERTEX_LIMIT 100
#define EDGE_LIMIT -1

void test_graph_transpose(void) {

	unsigned int Failures = CU_get_number_of_failures();

	for(size_t I = 0; I < 1000; I++) {

		struct graph * G = graph_random(VERTEX_LIMIT, EDGE_LIMIT), * T = NULL;
		CU_ASSERT_PTR_NOT_NULL(G);

		T = graph_transpose(G);
		CU_ASSERT_PTR_NOT_NULL(T);

		CU_ASSERT_EQUAL(T->Vertices, G->Vertices);

		//Test that G edge destinations as T edge sources point to the vertex of that edge in G
		size_t T_Vertex_Edge_Iterator[G->Vertices];
		for(size_t V = 0; V < G->Vertices; V++) T_Vertex_Edge_Iterator[V] = 0;
		//fprintf(stderr, "\nGraph %lu\n\n", I);
		for(size_t V = 0; V < G->Vertices; V++) {
			for(size_t E = 0; E < G->Vertex[V].Edges; E++) {
				#define G_DEST G->Vertex[V].Edge[E].Destination
				//fprintf(stderr, "%lu %lu : TV = %lu\n", V, T->Vertex[G_DEST].Edge[T_Vertex_Edge_Iterator[G_DEST]].Destination, G_DEST);
				CU_ASSERT_EQUAL(V, T->Vertex[G_DEST].Edge[T_Vertex_Edge_Iterator[G_DEST]++].Destination);
				#undef G_DEST
			}
		}

		graph_free(T);
		graph_free(G);

		//Problem?:
		CU_ASSERT_EQUAL_FATAL(Failures, CU_get_number_of_failures());
	}
}

void test_graph_random(void) {

	unsigned int Failures = CU_get_number_of_failures();
	srand(time(0));	

	for(size_t I = 0; I < 1000; I++) {

		struct graph * G = graph_random(VERTEX_LIMIT, EDGE_LIMIT);
		CU_ASSERT_PTR_NOT_NULL(G);
		CU_ASSERT(G->Vertices < VERTEX_LIMIT);

		for(size_t V = 0; V < G->Vertices; V++) {
			CU_ASSERT(G->Vertex[V].Edges < G->Vertices);

			if(G->Vertex[V].Edges > 0) CU_ASSERT(G->Vertex[V].Edge[0].Destination < G->Vertices);
			for(size_t E = 1; E < G->Vertex[V].Edges; E++) {
				CU_ASSERT(G->Vertex[V].Edge[E].Destination < G->Vertices);
				CU_ASSERT(G->Vertex[V].Edge[E].Destination > G->Vertex[V].Edge[E - 1].Destination);
			}
		}
		
		graph_free(G);

		CU_ASSERT_EQUAL_FATAL(Failures, CU_get_number_of_failures());
	}
}

int init_suite_1(void) { return 0; }

int clean_suite_1(void) { return 0; }

int main(void) {

	if(CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	CU_pSuite Suite_1 = CU_add_suite("Suite_1", init_suite_1, clean_suite_1);
	if(!Suite_1) return CU_cleanup_registry(), CU_get_error();

	if(!CU_add_test(Suite_1, "Test of graph_random", test_graph_random)
	|| !CU_add_test(Suite_1, "Test of graph_transpose", test_graph_transpose))
		return CU_cleanup_registry(), CU_get_error();

	return CU_basic_set_mode(CU_BRM_VERBOSE), CU_basic_run_tests(), CU_cleanup_registry(), CU_get_error();
}
