//Zero Hanami (C) 2021: graph_transpose.c

#include "0hana/graph.h"

struct graph * graph_transpose(struct graph const * const Original) {
	#define O Original
	struct graph * const Transpose = (struct graph *)malloc(sizeof(struct graph));
	#define T Transpose
	if(!T) return (struct graph *)(NULL);

	_relax(T->Node, struct graph_node *) = (struct graph_node *)malloc(sizeof(struct graph_node) * O->Nodes);
	if(!T->Node) return (struct graph *)(free(T), NULL);

	//Determine the number of transpose links
	_relax(T->Nodes, size_t) = O->Nodes;
	for(size_t N = 0; N < T->Nodes; N++) _relax(T->Node[N].Links, size_t) = 0;

	for(size_t N = 0; N < T->Nodes; N++)
		for(size_t L = 0; N < O->Node[N].Links; L++) _relax(T->Node[O->Node[N].Link[L].Term].Links, size_t) += 1;

	//Allocate the link counts
	for(size_t N = 0; N < T->Nodes; N++) {
		_relax(T->Node[N].Link, struct graph_link *) = (struct graph_link *)malloc(sizeof(struct graph_link) * T->Node[N].Links);
		if(!T->Node[N].Link) {
			while(N-- > 0) free(T->Node[N].Link);
			return (struct graph *)(free(T->Node), free(T), NULL);
		}

		//Initialize link variables to enable graph_free(T) cleanup if malloc fails ahead
		for(size_t L = 0; L < T->Node[N].Links; L++) {
			_relax(T->Node[N].Link[L].Term, size_t)   =   -1;
			_relax(T->Node[N].Link[L].Reals, size_t)  =    0;
			_relax(T->Node[N].Link[L].Real, double *) = NULL;
		}
	}

	//Make an iterator array for determining which transpose link needs to be assigned its original counter part
	size_t * const Iterator = (size_t *)malloc(sizeof(size_t) * T->Nodes);
	#define I Iterator
	if(!I) return (struct graph *)(graph_free(T), NULL);

	//Initialize the link iterators
	for(size_t N = 0; N < T->Nodes; N++) I[N] = 0;

	//Assign the counterparts
	for(size_t N = 0; N < O->Nodes; N++)
		for(size_t L = 0; L < O->Node[N].Links; L++) {
			#define X O->Node[N].Link[L].Term
			_relax(T->Node[X].Link[I[X]].Term, size_t) = O->Node[N].Link[L].Term;
			_relax(T->Node[X].Link[I[X]].Reals, size_t) = O->Node[N].Link[L].Reals;
			_relax(T->Node[X].Link[I[X]].Real, double *) = (double *)malloc(sizeof(double) * T->Node[X].Link[I[X]].Reals);
			if(!T->Node[X].Link[I[X]].Real) return (struct graph *)(graph_free(T), free(I), NULL);

			for(size_t R = 0; R < T->Node[X].Link[I[X]].Reals; R++) _relax(T->Node[X].Link[I[X]].Real[R], double) = O->Node[N].Link[L].Real[R];
			I[X]++;
			#undef X
		}

	return (struct graph *)(free(I), T);
	#undef I
	#undef T
	#undef O
}
