//Zero Hanami (C) 2021: queue.h

#include <stdlib.h>

struct queue {
	struct queue_node {
		struct queue_node * Next;
		size_t Data;
	} * Head, * Tail;
};//struct queue Q = { .Head = NULL, .Tail = NULL };
//idea: retry status return feedback recall {
//	external function: ...
//		int retry = non_deterministic_function(..., 0);
//		if(condition for retry) int retry = non_deterministic_function(..., retry);
//		...
//
//	non_deterministic_function(..., int retry) {
//		switch(retry) {
//			...
//			case retryX: ...
//			...
//		}
//	}
//}


struct queue_node * queue_push(struct queue * const Q, size_t const Push);

size_t queue_pop(struct queue * const Q);
	
void queue_free(struct queue * const Q);
