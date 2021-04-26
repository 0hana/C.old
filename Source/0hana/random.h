//Zero Hanami (C) 2021: random.h

#ifndef _random
#define _random

#include <time.h>
#include "memory.h"

#define random rand()

size_t * random_unique_indexes_array(size_t const Indexes, size_t Variation);

#endif//_random
