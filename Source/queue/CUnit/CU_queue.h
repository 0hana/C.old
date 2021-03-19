#ifndef _CU_QUEUE
#define _CU_QUEUE
#include <CUnit/CUnit.h>
#include "0hana/queue/queue.h"

int init_queue_suite(void);
int clean_queue_suite(void);

void CU_queue_free(void);

#endif//_CU_QUEUE
