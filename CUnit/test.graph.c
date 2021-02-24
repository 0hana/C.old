//Zero Hanami (C) 2021: test.graph.c

#include <stdio.h>
#include "test.graph.h"
#define VERTEX_LIMIT 100
#define EDGE_LIMIT -1

int init_graph_suite(void) { return 0; }
int clean_graph_suite(void) { return 0; }

void test_graph_random(void) {

	unsigned int const Failures = CU_get_number_of_failures();
	time_t Seed = time(0);
	srand(Seed);

	printf(" (Seed = %lu) ", Seed);

	for(size_t I = 0; I < 1000; I++) {

		struct graph * const G = graph_random(VERTEX_LIMIT, EDGE_LIMIT);
		CU_ASSERT_PTR_NOT_NULL_FATAL(G);
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

void test_graph_transpose(void) {

	unsigned int const Failures = CU_get_number_of_failures();

	for(size_t I = 0; I < 1000; I++) {

		struct graph * const G = graph_random(VERTEX_LIMIT, EDGE_LIMIT);
		CU_ASSERT_PTR_NOT_NULL_FATAL(G);

		struct graph * const T = graph_transpose(G);
		if(!T) {
			graph_free(G);
			CU_FAIL_FATAL("Failed to allocate Transpose graph -- stopping test");
		}

		CU_ASSERT_EQUAL(T->Vertices, G->Vertices);

		//Test that G edge destinations as T edge sources point to the vertex of that edge in G
		size_t T_Vertex_Edge_Iterator[G->Vertices];
		for(size_t V = 0; V < G->Vertices; V++) T_Vertex_Edge_Iterator[V] = 0;
		for(size_t V = 0; V < G->Vertices; V++) {
			for(size_t E = 0; E < G->Vertex[V].Edges; E++) {
				#define G_DEST G->Vertex[V].Edge[E].Destination
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

void test_graph_bfs(void) {
	/*
	Need to confirm:
		- all parents in Data->Parent are < G->Vertices
		- backtracking Parent array for a given index yields a valid path in the graph G
			- for each non-source vertex NSV in G where Data->Distance[V] != (size_t)-1, there exists an edge E such that G->Vertex[Parent[NSV]].Edge[E].Destination == NSV
		- all distances in Data->Distance are either < (G->Vertices - 1) or == (size_t)-1
	*/

	unsigned int const Failures = CU_get_number_of_failures();

	for(size_t I = 0; I < 1000; I++) {

		struct graph * const G = graph_random(VERTEX_LIMIT, EDGE_LIMIT);
		CU_ASSERT_PTR_NOT_NULL_FATAL(G);

		if(G->Vertices > 0) {
			size_t const Source = rand() % G->Vertices;
			struct graph_bfs_data * const Data = graph_bfs(G, Source);

			CU_ASSERT_TRUE(Data->Distance[Source] == 0 && Data->Parent[Source] == Source);

			//A vertex should only equal its parent if it is the Source (Data->Distance[Source] == 0) or unreachable from the Source (Data->Distance[V] == -1)
			//Test that Distance to Source from a vertex V equals the number of edges between the two by backtracking the Parent array
			for(size_t V = 1; V < G->Vertices; V++) {
				#define NON_SOURCE_VERTEX (Source + V) % G->Vertices
				#define IFF(A, B) ((A || !B) && (!A || B))

				CU_ASSERT_TRUE(IFF((Data->Distance[NON_SOURCE_VERTEX] == (size_t)-1), (Data->Parent[NON_SOURCE_VERTEX] == NON_SOURCE_VERTEX)));

				//Each parent in backtracking should only be visited once, or it is not a path (and therefore not a shortest path--considering only edge count) by definition
				enum boolean Visited[G->Vertices];
				for(size_t U = 0; U < G->Vertices; U++) Visited[U] = false;

				size_t Parent_Vertex = Data->Parent[NON_SOURCE_VERTEX];
				size_t Counter = (size_t)-1;

				Visited[NON_SOURCE_VERTEX] = true;
				Visited[Parent_Vertex] = true;

				if(Parent_Vertex != NON_SOURCE_VERTEX) {
					Counter = 1;

					enum boolean Valid_Edge = false;
					for(size_t E = 0; E < G->Vertex[Parent_Vertex].Edges; E++) {
						if(G->Vertex[Parent_Vertex].Edge[E].Destination == NON_SOURCE_VERTEX) Valid_Edge = true;
					}
					CU_ASSERT_TRUE(Valid_Edge);

					while(Parent_Vertex != Data->Parent[Parent_Vertex]) {
						Counter++;

						Valid_Edge = false;
						for(size_t E = 0; E < G->Vertex[Data->Parent[Parent_Vertex]].Edges; E++) {
							if(G->Vertex[Data->Parent[Parent_Vertex]].Edge[E].Destination == Parent_Vertex) Valid_Edge = true;
						}
						CU_ASSERT_TRUE(Valid_Edge);

						Parent_Vertex = Data->Parent[Parent_Vertex];

						//Test that this new Parent_Vertex has not yet been visited during backtracking toward the Source
						CU_ASSERT_FALSE(Visited[Parent_Vertex]);
						Visited[Parent_Vertex] = true;
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
