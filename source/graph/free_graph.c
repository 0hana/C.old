/* Hanami Zero (C) 2021: graph/free_graph.c */
#include "0hana/graph.h"

v free_graph(graph * c G)
{
	#define V Vertex
	#define Vs Vertexes
	for(x V = 0; V < G->Vs; V++)
	{
		#define A Adjacency
		#define As Adjacencyes
		for(x A = 0; A < G->V[V].As; A++)
		{
			if(G->V[V].A[A].Edge) f(G->V[V].A[A].Edge);
		}
		f(G->V[V].A);
		#undef  As
		#undef  A
	}
	f(G->V), f(G);
	#undef  Vs
	#undef  V
}


#ifdef  test

test
{
	graph * c G = m(s(graph));
	relax(G->Vertexes,x) = 16;
	relax(G->Vertex,graph_vertex *) = m(s(graph_vertex) * G->Vertexes);
	for(x V = 0; V < G->Vertexes; V++)
	{
		relax(G->Vertex[V].Adjacencyes,x) = 16;
		relax(G->Vertex[V].Adjacency,graph_adjacency *) = m(s(graph_adjacency) * G->Vertex[V].Adjacencyes);
		for(x A = 0; A < G->Vertex[V].Adjacencyes; A++)
		{
			relax(G->Vertex[V].Adjacency[A].Edges,x) = 16;
			relax(G->Vertex[V].Adjacency[A].Edge,r *) = m(s(r) * G->Vertex[V].Adjacency[A].Edges);
		}
	}
	free_graph(G);
	subtest(pass);  // valgrind memcheck will determine if a leak occured
}

#endif//test
