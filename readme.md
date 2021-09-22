# 0hana/C  [![build and test](https://github.com/0hana/C/actions/workflows/build-and-test.yml/badge.svg?branch=main)](https://github.com/0hana/C/actions/workflows/build-and-test.yml)
Personal C Utility APIs

## Setup
Expectations:
- You are on a [`Linux`](https://github.com/torvalds/linux) host (virtual machines are fine)
- You have GNU [`make`](https://www.gnu.org/software/make/) installed
- At least one of the following is true:
  - [`docker`](https://docs.docker.com/get-started/) is installed and your user is a member of the `docker` group
  - The relevant dependencies are installed (those relevant to the container are listed in the [`makefile`](makefile), in the `dependencies` variable assignment)

## Docker container setup
Run `make`

## Building object files
Run `make`

## Running unit tests
Run `make`

## make
Remembering to do ***"that one thing"*** before you do ***"that other thing"*** is extremely error prone.
It can also be difficult to reorganize code without breaking a good deal of it.
So `make` and methodology handle all ***"that"***.

What does `make` do (using [`makefile`](makefile))?
- Determine if we want to `make` in a container (default), and if so:
  - If a docker container image is not present, generate one
  - If a docker container       is not present, generate one from the image
  - Copy relevant files into the container (e.g. the [`source`](source) directory and its contents)
  - Run `make` inside the container but skip the above steps
- Mirror a `build` directory structure against the [`source`](source) directory structure (this does not delete files)
- If there exists a `.c` file in [`source`](source) that doesn't have a corresponding `.o` file in `build`, or vice-versa, (re)generate a unit testing main function file `build/0hana_test_dispatch.c`
- Remove all files in `build` (if it exists) that do not have a corresponding `.c` file in [`source`](source) (this includes `.d`, `.o`, `.s`, and `.log` files)
- Translate all new or modified `.c` files in `source` to a `.o` file along the mirrored path in `build` (`.d` and `.s` files accompany each `.o`)
- Synthesize `build/0hana_test_dispatch` from `build/0hana_test_dispatch.c` and all `.o` files in `build`, and Execute through `valgrind --tool=memcheck` (`valgrind`/`memcheck` is used to watch for memory leaks that may occur during testing)

## methodology
Every function declared in a `.h` file in [`source`](source) is defined in its own file named for itself. I find this personally helps me focus on only the function that matters in a moment, but it also greatly simplifies the build and testing system. It makes identifying user written functions trivial, which can then be used to generate other intermittent source code, such as in `build/0hana_test_dispatch`, and tracing dependencies between user written functions via their `gcc` assembly files with `grep call <function_source_file> | grep -v $(basename <function_source_file>) | grep <user_function1|user_function2|...> | sort -u`.

I previously used `cunit` to test my work, but found it awkward and overly complex for my purposes. Too many useless macros, too rigid a framework, no parallel testing (without considerable hacking).
So I experimented a little, and found I could do most everything I wanted with 2 user macros, and the `__LINE__` and `__FILE__` built in preprocessor macros.
Since then, I have expanded the macros a little, and now most come from the `makefile`.

Briefly, the only macros of interest for developing code here are `test` and `subtest`.

### Writing a function test
Despite taking weeks to devise and refine what I wanted out of the automated build and testing system (via `make`), it has made forward development much, much smoother.
To write a `test` for a function, follow this format in the relevant function file:

	#ifdef  test
	
	test {
		/* code goes here */
	}
	
	#endif//test

At a high level, that's all there is to it.
A little deeper, here's the [`copy`](source/memory/copy.c) source file as an example:

	/* Hanami Zero (C) 2021: memory/copy.c */
	#include "0hana/memory.h"
	
	v copy(x Size, o c Copy, v c * c Original) { while(Size-- > 0) { ((e*)Copy)[Size] = ((e*)Original)[Size]; } }
	
	
	#ifdef  test
	
	test {
		#define Number 0x10000
	
		int Original[Number];
		int     Copy[Number];
	
		for(x X = 0; X < Number; X++) {
			Original[X] = X * X;
			    Copy[X] =     0;
		}
	
		copy(s(int) * Number, Copy, Original);
	
		for(x X = 0; X < Number; X++) {
			if(subtest(Copy[X] == Original[X], "check loop iteration: %lu", X));
			else return;
		}
	}
	
	#endif//test

The [`copy`](source/memory/copy.c) function copies the contents of one contiguous block of memory to another of equal memory size.
The `test` of [`copy`](source/memory/copy.c) proceeds as follows:
- Creates 2 blocks memory to hold 65536 (the same as 0x10000) `int` type variables each
  - The first block, `Original`, has each `int` initialized to the square of its index
  - The second block, `Copy`, has each `int` initialized to 0
- The [`copy`](source/memory/copy.c) function is applied to `Copy` and `Original`
- To determine whether or not the [`copy`](source/memory/copy.c) function had the intended behavior (make the memory of `Copy` identical to the memory of `Original`) we use `subtest`

#### `subtest`
`subtest` is more or less the core macro underpinning the entire test system.
`test` used to be more involved, but now just makes the presence of a variable transparent and reduces end user typing.
The key difference now is that `test` is defined by [`makefile`](makefile) on a per file basis at make time, whereas `subtest` is still defined in [`source/types.h`](source/types.h).
`subtest` behaves essentially as a boolean (true/false) function that returns whatever it's first parameter evaluates to, and it only requires that first parameter.

In the code above, we have:

	subtest(Copy[X] == Original[X], "check loop iteration: %lu", X)

It could have been written without the extra parameters:

	subtest(Copy[X] == Original[X])

If it had, then all `subtest` would do is determine whether or not the contained expression `Copy[X] == Original[X]` evaluated to true or false.
- If false, it writes the enclosed expression, its [`source`](source) file, and line number to a log file in `build` mirroring its [`source`](source) path (e.g. `source/memory/copy.c` -> `build/memory/copy.log`) and returns false
- If true, it returns true
The log file stream is closed by the test dispatch main function in `build/0hana_test_dispatch.c`

If the `subtest` failed on loop iteration 1234, then the log file `build/memory/copy.log` would read:

	source/memory/copy.c: Line 23: Failed subtest: `Copy[X] == Original[X]`

With the additional parameters, `subtest` does, in addition to the above, essentially a printf to the log file directly following the failed expression notice, and would read:

	source/memory/copy.c: Line 23: Failed subtest: `Copy[X] == Original[X]`
	- check loop iteration: 1234

If no `subtest` fails (evaluates a false expression), then a `.log` file corresponding to the function source file will be deleted (provided one exists).
- The presence of `.log` files inform `build/0hana_test_dispatch` whether or not to re-test a function.
