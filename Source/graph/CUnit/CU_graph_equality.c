//Zero Hanami (C) 2021: CU_graph_equality.c

#include "0hana/CUnit/CU_graph.h"

void CU_graph_equality(void) {
	struct graph Static0 = { .Nodes = 0, .Node = NULL };

	CU_ASSERT_TRUE(graph_equality(&Static0, &Static0));

	struct graph_link S1N1L = {
		.Term = 0, .Reals = 0, .Real = NULL
	};

	struct graph_node S1N[2] = {
		{ .Links = 0, .Link = NULL },
		{ .Links = 1, .Link = &S1N1L }
	};

	struct graph Static1 = { .Nodes = 1, .Node = S1N };

	CU_ASSERT_FALSE(graph_equality(&Static0, &Static1));
	CU_ASSERT_FALSE(graph_equality(&Static1, &Static0));
	CU_ASSERT_TRUE(graph_equality(&Static1, &Static1));

	double Real0[2] = { 3.14, 2.79 };

	struct graph_link Link0[2] = {
		{ .Term = 0, .Reals = 0, .Real = NULL },
		{ .Term = 1, .Reals = 2, .Real = Real0 }
	};

	struct graph_link Link1[2] = {
		{ .Term = 0, .Reals = 2, .Real = Real0 },
		{ .Term = 1, .Reals = 0, .Real = NULL }
	};

	struct graph_node Node[2] = {
		{ .Links = 2, .Link = Link0 },
		{ .Links = 2, .Link = Link1 }
	};

	struct graph Static2 = { .Nodes = 2, .Node = Node };

	CU_ASSERT_FALSE(graph_equality(&Static0, &Static2));
	CU_ASSERT_FALSE(graph_equality(&Static2, &Static0));
	CU_ASSERT_FALSE(graph_equality(&Static1, &Static2));
	CU_ASSERT_TRUE(graph_equality(&Static2, &Static2));
}
