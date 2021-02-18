//Zero Hanami (C) 2021: queue.c

#include "queue.h"

struct queue_node * queue_push(struct queue * const Q, size_t const Push) {
	if(!Q->Tail) {
		Q->Tail = malloc(sizeof(struct queue_node));
		if(!Q->Tail) return NULL;
		Q->Head = Q->Tail;
	}
	else if(!(Q->Tail->Next = malloc(sizeof(struct queue_node)))) return NULL;
	else Q->Tail = Q->Tail->Next;
	Q->Tail->Next = NULL;
	Q->Tail->Data = Push;
	return Q->Tail;
}

void queue_pop(struct queue * const Q) {
	if(!Q->Head) return;
	struct queue_node * H = Q->Head;
	Q->Head = Q->Head->Next;
	free(H);
	if(Q->Tail == H) Q->Tail = NULL;
}
