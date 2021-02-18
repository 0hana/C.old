# C
0hana C Utilities

## Compilation plan
The Build directory is a cache of local build objects to speed up recompilation for retest

Test groups themselves will be subdivided into independently compilable groups under `CUnit/test.<name>.c`

The main 'test' executable will take CLI parameters to specify which tests to run or _not_ run.

Specifying nothing will run all tests. Specifying `all` will do the same.

Specifying nothing after an inverse (TBD) token will run all tests. Specifying `all` after an inverse token will run _no_ tests.

Specifying `graph` will run the tests on the test.graph.c defined tests.

Specifying an inverse token and then `graph` will run all tests but graph.

This should be sufficiently granular in test control and speed up for a while.
