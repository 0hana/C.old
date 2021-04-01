//Zero Hanami (C) 2021: types.h

#ifndef _TYPES
#define _TYPES

#define _(term) _##term
#ifndef _const
#define _const const
#define _override(type, A, B) *(type *)&A = (type)B
#else
#define _override(type, A, B) A = (type)B
#endif/*_const --
	* Must either be an empty definition or defined as 'const'
	* Provide empty definition above or,
	* using gcc or clang enter 'cc -D _const= '
	to remove compilation guards on _(const) terms
	(doing so reduces _override to a simple assignment expression).
*/
#ifndef _sepples
#define _bool bool
#define _true true
#define _false false
#else //e_e
#define bool _bool
#define true _true
#define false _false
#endif//_sepples

typedef enum binary {
	binary_0 = 0, binary_1 = ~0,
	_(true) = binary_1, _(false) = binary_0
} _(bool);

typedef enum ternary {
	ternary_0 = 0, ternary_1 = +1, ternary_2 = -1,
	above = ternary_1, below = ternary_2, comparable = ternary_0
	/*
		For those of you to whom the semantics (the meaning) of the term
		'comparable' is unclear in this context, consider what it means
		for 2 objects to be equal --
		
		thinking of a pair of integers and a pair of C strings may be helpful:			

		#Ex.

		int const A = 0, B = 0, C = 1;


		if(A == B) printf("%i equals %i\n", A, B);//This will print
		else printf("%i does NOT equal %i\n", A, B);//This will NOT

		if(A == C) printf("%i equals %i\n", A, C);//This will NOT print
		else printf("%i does NOT equal %i\n", A, C);//This will


		char lower_case_text[] = "hi there friend";
		char UPPER_CASE_TEXT[] = "HI THERE FRIEND";

		//Now we set all the letters of UPPER_CASE_TEXT to lower case

		for(size_t Letter = 0; Letter < sizeof(UPPER_CASE_TEXT); Letter++)
			UPPER_CASE_TEXT[Letter] = tolower(UPPER_CASE_TEXT[Letter]);

		printf("%s\n", lower_case_text);
		printf("%s\n", UPPER_CASE_TEXT);

		if(lower_case_text == UPPER_CASE_TEXT)
			printf("The following 2 strings are equal:\n%s\n%s\n", lower_case_text, UPPER_CASE_TEXT);
		else
			printf("The following 2 strings are NOT equal:\n%s\n%s\n", lower_case_text, UPPER_CASE_TEXT);
		
		//The print will read "
		//The following 2 strings are NOT equal:
		//hi there friend
		//hi there friend
		//"

		#end

		Here, we want to ask if 2 pieces of text, lower_case_text and UPPER_CASE_TEXT, match or not.
		After setting all the letters in UPPER_CASE_TEXT to their lower case forms, they do match,
		and all equivalently indexed letters in lower_case_text are equal to those in UPPER_CASE_TEXT,
		yet,
		UPPER_CASE_TEXT does NOT equal lower_case_text.
		This is because UPPER_CASE_TEXT is a separate object
		from lower_case_text.
		2 people may share the same name and still be different people.

		A less philosophical view (and perhaps more accurate to this particular example)
		is that lower_case_text and UPPER_CASE_TEXT both hold values referencing positions in memory,
		and they reference different positions.
		We merely editted the values starting at the position held by UPPER_CASE_TEXT to match
		those starting at the position held by lower_case_text.
		The positions referenced (and then checked for equality) didn't change, thus the 2 values
		are not equal.
	*/
} (* order)(void * O1, void * O2);

#endif//_TYPES
