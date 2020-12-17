//Zero Hanami (C) 2020
#ifndef common_h
#define common_h

#ifdef DEBUG
#include <assert.h>
#endif

#include <stdlib.h>

enum binary { binary_0 = 0, binary_1 = ~0 };

#define c const
#define s size_t
#define u char
#define v void
#define z ssize_t

typedef enum ternary { ternary_0 = 0, ternary_1 = +1, ternary_2 = -1 } (*ternary_relation)(v c * c, v c * c);

enum ternary s_ternary_relation(s c * c, s c * c);

enum ternary z_ternary_relation(z c * c, z c * c);

v copy(s, v * c, v c * c);

v swap(s, v * c, v * c);

#define IMPLY(A, B) (!(A) || (B))
#endif//common_h
