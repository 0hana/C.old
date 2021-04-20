//Zero Hanami (C) 2021: graph_allocate.c

#include "0hana/graph.h"

struct graph * graph_allocate(size_t const Nodes, size_t const * const Links, size_t const * const * const Reals) {
	//Protect against Dragons
	if((Reals && !(Links || Nodes)) || (Links && !Nodes)) return NULL;

	//Allocate initial graph structure
	struct graph * Graph = (struct graph *)malloc(sizeof(struct graph));
	if(!Graph) return NULL;

	//Assign initial graph structure values
	_relax(Graph->Nodes, size_t) = 0;
	_relax(Graph->Node, struct graph_node *) = NULL;

	if(Nodes) {
		_relax(Graph->Node, struct graph_node *) = graph_node_allocate(Nodes);
		if(!Graph->Node) return (struct graph *)(graph_free(Graph), NULL);

		_relax(Graph->Nodes, size_t) = Nodes;
		if(Links) {
			for(size_t N = 0; N < Graph->Nodes; N++) {
				_relax(Graph->Node[N].Link, struct graph_link *) = graph_link_allocate(Links[N]);
				if(!Graph->Node[N].Link) return (struct graph *)(graph_free(Graph), NULL);

				_relax(Graph->Node[N].Links, size_t) = Links[N];
				if(Reals) {
					for(size_t L = 0; L < Graph->Node[N].Links; L++) {
						_relax(Graph->Node[N].Link[L].Real, double *) = (double *)malloc(sizeof(double) * Reals[N][L]);
						if(!Graph->Node[N].Link[L].Real) return (struct graph *)(graph_free(Graph), NULL);

						_relax(Graph->Node[N].Link[L].Reals, size_t) = Reals[N][L];
					}
				}
			}
		}
	}

	return Graph;
}
