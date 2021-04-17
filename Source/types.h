//Zero Hanami (C) 2021: types.h

#ifndef _types
#define _types

#define _(term) _##term

#ifndef _const
#define _const const
#define _relax(A, type) *(type *)&A
#else
#define _const
#define _relax(A, type) A
#endif//_const

#define bool _bool
#define true _true
#define false _false

typedef enum binary {
	binary_0 =  0,
	binary_1 = ~0,
	true  = binary_1,
	false = binary_0
} bool;

enum ternary {
	ternary_0 =  0,
	ternary_1 = +1,
	ternary_2 = -1,
	above = ternary_1,
	below = ternary_2,
	equal = ternary_0
};

typedef enum ternary (* order)(void * O1, void * O2);

#endif//_types
