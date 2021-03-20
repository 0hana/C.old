//Zero Hanami (C) 2021: CU_graph_bfs.c

#include <stdio.h>
#include "CU_graph.h"
#include "0hana/random/random.h"
#define VERTEX_LIMIT 10
#define EDGE_LIMIT -1

void CU_graph_bfs(void) {
	/*
	Need to confirm:
		- all parents in Data->Parent are < G->Terms
		- backtracking Parent array for a given index yields a valid path in the graph G
			- for each non-source vertex NSV in G where Data->Distance[V] != (size_t)-1, there exists an edge E such that G->Term[Parent[NSV]].Link[E].Term == NSV
		- all distances in Data->Distance are either < (G->Terms - 1) or == (size_t)-1
	*/

	unsigned int const Failures = CU_get_number_of_failures();

	for(size_t I = 0; I < 100; I++) {

		struct graph * const G = graph_random(VERTEX_LIMIT, EDGE_LIMIT);
		CU_ASSERT_PTR_NOT_NULL_FATAL(G);

		if(G->Terms > 0) {
			size_t const Source = rand() % G->Terms;
			struct graph_bfs_data * const Data = graph_bfs(G, Source);

			CU_ASSERT_TRUE(Data->Distance[Source] == 0 && Data->Parent[Source] == Source);

			//A vertex should only equal its parent if it is the Source (Data->Distance[Source] == 0) or unreachable from the Source (Data->Distance[V] == -1)
			//Test that Distance to Source from a vertex V equals the number of edges between the two by backtracking the Parent array
			for(size_t V = 1; V < G->Terms; V++) {
				#define NON_SOURCE_VERTEX (Source + V) % G->Terms
				#define IFF(A, B) ((A || !B) && (!A || B))

				CU_ASSERT_TRUE(IFF((Data->Distance[NON_SOURCE_VERTEX] == (size_t)-1), (Data->Parent[NON_SOURCE_VERTEX] == NON_SOURCE_VERTEX)));

				//Each parent in backtracking should only be visited once, or it is not a path (and therefore not a shortest path--considering only edge count) by definition
				enum boolean Visited[G->Terms];
				for(size_t U = 0; U < G->Terms; U++) Visited[U] = false;

				size_t Parent_Term = Data->Parent[NON_SOURCE_VERTEX];
				size_t Counter = (size_t)-1;

				Visited[NON_SOURCE_VERTEX] = true;
				Visited[Parent_Term] = true;

				if(Parent_Term != NON_SOURCE_VERTEX) {
					Counter = 1;

					enum boolean Valid_Link = false;
					for(size_t E = 0; E < G->Term[Parent_Term].Links; E++) {
						if(G->Term[Parent_Term].Link[E].Term == NON_SOURCE_VERTEX) Valid_Link = true;
					}
					CU_ASSERT_TRUE(Valid_Link);

					while(Parent_Term != Data->Parent[Parent_Term]) {
						Counter++;

						Valid_Link = false;
						for(size_t E = 0; E < G->Term[Data->Parent[Parent_Term]].Links; E++) {
							if(G->Term[Data->Parent[Parent_Term]].Link[E].Term == Parent_Term) Valid_Link = true;
						}
						CU_ASSERT_TRUE(Valid_Link);

						Parent_Term = Data->Parent[Parent_Term];

						//Test that this new Parent_Term has not yet been visited during backtracking toward the Source
						CU_ASSERT_FALSE(Visited[Parent_Term]);
						Visited[Parent_Term] = true;
					}
					CU_ASSERT_EQUAL(Data->Distance[NON_SOURCE_VERTEX], Counter);
				}
				#undef IFF
				#undef NON_SOURCE_VERTEX
			}

			graph_bfs_data_free(Data);
		}

		graph_free(G);

		CU_ASSERT_EQUAL_FATAL(Failures, CU_get_number_of_failures());
	}
}
