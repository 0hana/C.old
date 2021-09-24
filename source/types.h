/* Hanami Zero (C) 2021: types.h */
#ifndef _types_
#define _types_

#include <stddef.h>

#define c const
#ifndef _const
#define _const const
#define relax(Constrained, type) (*(type *)&(Constrained))
#else
#define _const
#define relax(Constrained, type) (Constrained)
#endif//_const
#define _c _const

#define s(type) (sizeof(type))
#define a(Variable) (&(Variable))

typedef size_t    x;  //indeX
typedef ptrdiff_t z;  //Zahlen
typedef char      e;  //Element
typedef double    r;  //Real
typedef void *    o;  //Object
typedef void      v;  //Void

#define  bool _bool
#define  true _true
#define false _false
#define is   ==
#define isnt !=

typedef enum binary {
	binary_0 =  0,
	binary_1 = ~0,
	true  = binary_1,
	false = binary_0
} bool;

enum ternary {
	ternary_0 =  0,
	ternary_1 = +1,
	ternary_2 = -1
};

typedef enum ternary (* contrastor)(o O1, o O2);

#ifdef  test

#include <stdio.h>
#include <string.h>

#define subtest(expression, /* additional_info */...) \
(\
	(expression) \
	? ((void)(*Log_File is NULL && remove(log_file)), true) \
	: \
	(\
		(void)\
		(\
			*Log_File is NULL \
		 	&& (*Log_File = fopen(log_file, "a")) \
			&& fprintf(stderr, "\b\b\b%s" function_name "  test: FAILED -- See " log_file "\n  ...", extra_spacing)\
		)\
		, fprintf(*Log_File, __FILE__ ": Line %i: Failed subtest: `" #expression "`\n", __LINE__)\
		__VA_OPT__(, fprintf(*Log_File, "- " __VA_ARGS__), fprintf(*Log_File, "\n"))\
		, false\
	)\
)

static inline bool pass(v) { return true; }

#define pass pass()

#endif//test
#endif//_types_
