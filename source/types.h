/* Hanami Zero (C) 2021: types.h */
#ifndef _types_
#define _types_

#include <stddef.h>
#include <stdint.h>

typedef unsigned char byte;
#define little_endian (((byte)(*(int16_t *)("?"))) == '?')

typedef size_t z;
#define s(variable) sizeof(variable)
typedef void * a;

typedef enum error { no_error = 0, no_memory = 1 } function;
#define returns ==

#define i const
#define o
#define io

void endian_mirror(i z Bytes, io a const Datum);

#ifndef unconst
#define unconst
#define L_PAREN (
#define R_PAREN )
#define u(type, name, ...) union { type __VA_OPT__(un)##const __VA_OPT__(L_PAREN* const) name __VA_OPT__(R_PAREN)__VA_OPT__((__VA_ARGS__)); type __VA_OPT__(L_PAREN*) name##_unconst __VA_OPT__(R_PAREN)__VA_OPT__((__VA_ARGS__)); }
#define relax(variable) (*(typeof(variable##_unconst)*)&(variable))
#else
#define L_PAREN (
#define R_PAREN )
#define u(type, name, ...) type __VA_OPT__(L_PAREN*) name __VA_OPT__(R_PAREN)__VA_OPT__((__VA_ARGS__))
#define relax(variable) (variable)
#endif//unconst

#define bool _bool
#define true _true
#define false _false

typedef enum binary { binary_0 = 0, binary_1 = 1, true = binary_1, false = binary_0 } binary, bool;
typedef enum ternary { ternary_0 = 0, ternary_1 = +1, ternary_2 = -1 } ternary;

#ifdef  test

#include <stdio.h>
#include <string.h>

#define subtest(expression, /* additional_info */...) \
(\
	(expression) \
	? ((void)(*Log_File == NULL && remove(log_file)), true) \
	: \
	(\
		(void)\
		(\
			*Log_File == NULL \
		 	&& (*Log_File = fopen(log_file, "a")) \
			&& fprintf(stderr, "\b\b\b%s" function_name "  [ test: FAILED ] -- See " log_file "\n  ...", extra_spacing)\
		)\
		, fprintf(*Log_File, __FILE__ ": Line %i: Failed subtest: (" #expression ")\n", __LINE__)\
		__VA_OPT__(, fprintf(*Log_File, "- " __VA_ARGS__), fprintf(*Log_File, "\n"))\
		, false\
	)\
)

static inline bool pass(void) { return true; }

#define pass pass()  /* This prevents a -Wunused-value compiler warning -- change 'pass()' to 'true' and see what I mean */
#endif//test

#endif//_types_
