//Zero Hanami (C) 2021: main.graph.c

#include <CUnit/Basic.h>
#include <stdio.h>
#include "CU_graph.h"

int main(int const Updates, char const * const Update[]) {

	printf("\nSource Updates (%i): ", Updates - 1);
	for(unsigned U = 1; U < (unsigned)Updates; U++) printf("%s ", Update[U]);
	printf("\n");
	fflush(stdout);

	if(CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	CU_pSuite Graph_Suite = CU_add_suite("Graph", init_graph_suite, clean_graph_suite);
	if(!Graph_Suite) return CU_cleanup_registry(), CU_get_error();

	if(!CU_add_test(Graph_Suite, "graph_random", test_graph_random)
	|| !CU_add_test(Graph_Suite, "graph_transpose", test_graph_transpose)
	|| !CU_add_test(Graph_Suite, "graph_bfs", test_graph_bfs))
		return CU_cleanup_registry(), CU_get_error();

	return CU_basic_set_mode(CU_BRM_VERBOSE), CU_basic_run_tests(), CU_cleanup_registry(), CU_get_error();
}
