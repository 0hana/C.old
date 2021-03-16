#include <stdio.h>
#include "CU_queue.h"

void CU_queue_free(void){

  struct queue Q = {.Head = NULL, .Tail = NULL};
  for(int i = 0; i < 10; i++){
    queue_push(&Q, i);
  }

  CU_ASSERT(Q.Head != NULL && Q.Tail != NULL);
  queue_free(&Q);
  CU_ASSERT(Q.Head == NULL && Q.Tail == NULL);
  
}
