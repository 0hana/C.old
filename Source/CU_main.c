//Zero Hanami (C) 2021: CU_main.c

#include <CUnit/Basic.h>
#include <stdio.h>
#include <time.h>
#include "0hana/CUnit/CU_graph.h"

int main(int words, char * word[]) {
	time_t Seed = time(0);
	srand(Seed);
	printf("Seed = %li\n", Seed);
	fflush(stdout);

	if(CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	CU_pSuite
		Graph_Suite;

	if(!(Graph_Suite = CU_add_suite("Graph", NULL, NULL)))
		return CU_cleanup_registry(), CU_get_error();

	if(!CU_add_test(Graph_Suite, "graph_equality", CU_graph_equality)
	|| !CU_add_test(Graph_Suite, "graph_clone", CU_graph_clone))
		return CU_cleanup_registry(), CU_get_error();

	return CU_basic_set_mode(CU_BRM_VERBOSE), CU_basic_run_tests(), CU_cleanup_registry(), CU_get_error();
}
