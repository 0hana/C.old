//Zero Hanami (C) 2021: graph.main.c

#include <CUnit/Basic.h>
#include "../graph.h"

int CLI_Words;
char const * const * CLI_Word;

void test_graph_transpose(void) {}

void test_graph_random(void) {}

int init_suite_1(void) { return 0; }

int clean_suite_1(void) { return 0; }

int main(int const Words, char const * const Word[]) {
	//Convey CLI input parameters to tests
	CLI_Words = Words;
	CLI_Word = Word;

	if(CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	CU_pSuite Suite_1 = CU_add_suite("Suite_1", init_suite_1, clean_suite_1);
	if(!Suite_1) return CU_cleanup_registry(), CU_get_error();

	if(!CU_add_test(Suite_1, "Test of graph_transpose", test_graph_transpose)
	|| !CU_add_test(Suite_1, "Test of graph_random", test_graph_random))
		return CU_cleanup_registry(), CU_get_error();

	return CU_basic_set_mode(CU_BRM_VERBOSE), CU_basic_run_tests(), CU_cleanup_registry(), CU_get_error();
}
