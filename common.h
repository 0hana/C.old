//Zero Hanami (C) 2020
#ifndef common_h
#define common_h

#include <stdlib.h>
/*
	For types size_t and ssize_t, as well as dynamic memory allocation functions
	malloc(), realloc(), and free()
*/
#ifdef DEBUG
#include <assert.h>
#include <stdio.h>
#define _Print_Error_(TEXT, ...) fprintf(stderr, TEXT "\n" __VA_OPT__(,) __VA_ARGS__)
#endif

#define c const
typedef void v;
typedef char u;
typedef size_t s; extern s c S;//S is the log2 of the s state space
typedef ssize_t z; extern z c Z;//Z is the z cast of the largest s value that multiplied by 2 is 0 (or the s state space divided by 2)

enum binary { binary_0 = 0, binary_1 = ~0 };
typedef enum ternary { ternary_0 = 0, ternary_1 = +1, ternary_2 = -1 } (*ternary_greater)(v c * c, v c * c);
/*
	ternary_greater functions cast to the appropriately typed pointers internally, and returns whichever value is deemed greater in order statistic value.
	The order is determined by the function, thus in sorting an array of unknown type, this function determines the type and ordering among items within.
*/
enum ternary S_Greater(v c * c S1, v c * c S2);//Casts to (s c * c) to dereference internally
enum ternary Z_Greater(v c * c Z1, v c * c Z2);//Casts to (z c * c) to dereference internally

v Copy(s Size, v * c O1, v c * c O2);
v Swap(s Size, v * c O1, v * c O2);

#endif//common_h
