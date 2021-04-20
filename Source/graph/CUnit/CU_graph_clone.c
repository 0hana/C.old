//Zero Hanami (C) 2021: CU_graph_clone.c

#include "CU_graph.h"

void CU_graph_clone(void) {

	struct graph Static0 = { .Nodes = 0, .Node = NULL };

	struct graph_link S1N1L = {
		.Term = 0, .Reals = 0, .Real = NULL
	};

	struct graph_node S1N[2] = {
		{ .Links = 0, .Link = NULL },
		{ .Links = 1, .Link = &S1N1L }
	};

	struct graph Static1 = { .Nodes = 1, .Node = S1N };

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

	struct graph * G = graph_clone(&Static0);
	if(!G) return;

	CU_ASSERT_TRUE(graph_equality(G, &Static0));
	CU_ASSERT_FALSE(graph_equality(G, &Static1));
	CU_ASSERT_FALSE(graph_equality(G, &Static2));

	graph_free(G);

	G = graph_clone(&Static1);
	if(!G) return;

	CU_ASSERT_FALSE(graph_equality(G, &Static0));
	CU_ASSERT_TRUE(graph_equality(G, &Static1));
	CU_ASSERT_FALSE(graph_equality(G, &Static2));

	graph_free(G);

	G = graph_clone(&Static2);
	if(!G) return;

	CU_ASSERT_FALSE(graph_equality(G, &Static0));
	CU_ASSERT_FALSE(graph_equality(G, &Static1));
	CU_ASSERT_TRUE(graph_equality(G, &Static2));

	graph_free(G);
}
