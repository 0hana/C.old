//Zero Hanami (C) 2021: CU_graph.h

#ifndef _CU_GRAPH
#define _CU_GRAPH
#include <CUnit/CUnit.h>
#include "0hana/graph/graph.h"

int init_graph_suite(void);
int clean_graph_suite(void);

void CU_graph_bfs(void);
void CU_graph_random(void);
void CU_graph_transpose(void);

#endif//_CU_GRAPH
