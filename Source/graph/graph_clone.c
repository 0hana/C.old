//Zero Hanami (C) 2021: graph_clone.c

#include "0hana/graph.h"

struct graph * graph_clone(struct graph const * const Original) {
	#define O Original
	struct graph * const Clone = (struct graph *)malloc(sizeof(struct graph));
	#define C Clone
	if(!C) return (struct graph *)(NULL);

	_relax(C->Node, struct graph_node *) = graph_node_allocate(O->Nodes);
	if(!C->Node) return (struct graph *)(free(C), NULL);

	_relax(C->Nodes, size_t) = O->Nodes;
	for(size_t N = 0; N < C->Nodes; N++) {
		_relax(C->Node[N].Link, struct graph_link *) = graph_link_allocate(O->Node[N].Links);
		if(!C->Node[N].Link) {
			//Release memory on failure
			while(N-- > 0) {
				for(size_t L = 0; L < C->Node[N].Links; L++) free(_relax(C->Node[N].Link[L].Real, double *));
				free(C->Node[N].Link);
			}
			return (struct graph *)(free(C->Node), free(C), NULL);
		}

		_relax(C->Node[N].Links, size_t) = O->Node[N].Links;
		for(size_t L = 0; L < C->Node[N].Links; L++) {
			//Allocate parallel real arrays
			_relax(C->Node[N].Link[L].Real, double *) = (double *)malloc(sizeof(double) * O->Node[N].Link[L].Reals);
			if(!C->Node[N].Link[L].Real) {
				//Release memory on failure
				while(N > 0) {
					while(L > 0) free(_relax(C->Node[N].Link[L--].Real, double *));
					free(C->Node[N].Link);
				}
				return (struct graph *)(free(C->Node), free(C), NULL);
			}

			_relax(C->Node[N].Link[L].Reals, size_t) = O->Node[N].Link[L].Reals;
			for(size_t R = 0; R < C->Node[N].Link[L].Reals; R++) {
				_relax(C->Node[N].Link[L].Real[R], double) = O->Node[N].Link[L].Real[R];
			}
		}
	}

	return C;
	#undef C
	#undef O
}
