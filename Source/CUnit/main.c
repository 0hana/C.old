//Zero Hanami (C) 2021: main.c

#include <CUnit/Basic.h>
#include <stdio.h>
#include <time.h>
#include "0hana/graph/CUnit/CU_graph.h"
#include "0hana/queue/CUnit/CU_queue.h"

int main(int const Updates, char const * const Update[]) {

	printf("Source Updates (%i): ", Updates - 1);
	for(unsigned U = 1; U < (unsigned)Updates; U++) printf("%s ", Update[U]);
	printf("\n");

	time_t Seed = time(0);
	srand(Seed);
	printf("Seed = %li\n", Seed);

	fflush(stdout);

	if(CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	CU_pSuite
		Graph_Suite,
		Queue_Suite;

	if(!(Queue_Suite = CU_add_suite("Queue", init_queue_suite, clean_queue_suite))
	|| !(Graph_Suite = CU_add_suite("Graph", init_graph_suite, clean_graph_suite)))
		return CU_cleanup_registry(), CU_get_error();

	if(!CU_add_test(Graph_Suite, "graph_random", CU_graph_random)
	|| !CU_add_test(Graph_Suite, "graph_transpose", CU_graph_transpose)
	|| !CU_add_test(Graph_Suite, "graph_bfs", CU_graph_bfs)

	|| !CU_add_test(Queue_Suite, "queue_free", CU_queue_free))
		return CU_cleanup_registry(), CU_get_error();

	return CU_basic_set_mode(CU_BRM_VERBOSE), CU_basic_run_tests(), CU_cleanup_registry(), CU_get_error();
}
