//Zero Hanami (C) 2021: test.graph.h

#ifndef _GRAPH_TEST
#define _GRAPH_TEST
#include <CUnit/CUnit.h>
#include "0hana.C/graph.h"

int init_graph_suite(void);
int clean_graph_suite(void);

void test_graph_random(void);
void test_graph_transpose(void);
void test_graph_bfs(void);

#endif//_GRAPH_TEST
