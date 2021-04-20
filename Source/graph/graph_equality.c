//Zero Hanami (C) 2021: graph_equality

#include "0hana/graph.h"

bool graph_equality(struct graph const * const A, struct graph const * const B) {
	if(A->Nodes != B->Nodes) return false;

	for(size_t N = 0; N < A->Nodes; N++) {
		if(A->Node[N].Links != B->Node[N].Links) return false;

		for(size_t L = 0; L < A->Node[N].Links; L++) {
			if(A->Node[N].Link[L].Term  != B->Node[N].Link[L].Term
			|| A->Node[N].Link[L].Reals != B->Node[N].Link[L].Reals) return false;

			for(size_t R = 0; R < A->Node[N].Link[L].Reals; R++)
				if(A->Node[N].Link[L].Real[R] != B->Node[N].Link[L].Real[R]) return false;
		}
	}

	return true;
}
