# Hanami Zero (C) 2021: makefile
# Synchronize build against source

.PHONY: clean contained container image docker run uncontained 

dependencies := coreutils gcc libc-dev make valgrind

contained: docker
	@docker cp license.txt hanami:/home
	@docker cp makefile    hanami:/home
	@docker cp readme.md   hanami:/home
	@docker cp source      hanami:/home
	@docker exec -w /home  hanami sh -c 'make -j uncontained'

docker: container

container: image
	@if ! docker ps | grep -q 'hanami'; then docker run -dti --rm --name hanami hanami; fi

image:
	@if ! docker images | grep -q 'hanami'; then printf 'FROM alpine:3\nRUN apk add $(dependencies) gdb tree' | docker build -t hanami -; fi

ifeq 'uncontained' '$(filter uncontained,$(MAKECMDGOALS))'

# Added source file detection
$(foreach file,$(shell find source -name '*.c'),$(shell if [ ! -f $(file:source/%.c=build/%.o) ]; then rm -f build/0hana_test_dispatch.c; fi; mkdir -p $(dir $(file:source/%=build/%))))

# (Re)Moved source file detection
$(foreach file,$(shell find build  -name '*.o'),$(shell if [ ! -f $(file:build/%.o=source/%.c) ]; then rm -f build/0hana_test_dispatch.c $(file) $(file:.o=.s) $(file:.o=.d) $(file:.o=.log); fi))

# Directory structure synchronization
$(foreach directory,$(shell find build -mindepth 1 -type d),$(shell if [ ! -d $(directory:build/%=source/%) ]; then rm -rf $(directory); fi))

endif

uncontained: run

required_tests  = $(sort $(notdir $(basename $(shell find build -name '*.log'))))

run: build/0hana_test_dispatch
	@echo "-    Executing : $<"
	@valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -q -s $< $(required_tests)

object_targets := $(patsubst source/%.c,build/%.o,$(shell find source -name '*.c'))

build/0hana_test_dispatch: build/0hana_test_dispatch.c $(object_targets)
	@echo "- Synthesizing : $@"
	@gcc -Wall -Wextra -Wpedantic -g -o $@ $^ -D test='test'

build/%.o: build/%.s
	@echo "-  Translating :  $< -> $@"
	@gcc -g -c $< -o $@
	@echo > $(@:.o=.log)

.SECONDARY: $(object_targets:.o=.s)

build/%.s: source/%.c
	@echo '-  Translating : $< -> $@'
	@gcc -Wall -Wextra -Wpedantic -g -o $@ $< -D test='v test_$(notdir $(basename $<))(FILE * * c Log_File)' -D log_file='"$(@:.s=.log)"' -D function_name='"$(notdir $(basename $<))"' -S -MMD

function_names := $(sort $(notdir $(basename $(object_targets))))
function_count := $(words $(function_names))
         comma := ,

build/0hana_test_dispatch.c: makefile
	@echo "-   Generating : $@"
	@printf > $@ \
	'/* Hanami Zero (C) 2021: 0hana_test_dispatch.c */\n'\
	'// Unit testing main function\n'\
	'$(patsubst %,\n#include "../%",$(shell find source -name '*.h'))\n'\
	'$(patsubst %,\nv test_%(FILE * * c Log_File);,$(function_names))\n'\
	'\n'\
	'#define _function_count_ $(function_count)\n'\
	'v (* c test[_function_count_])(FILE * * c Log_File) = { $(patsubst %,test_%$(comma),$(function_names)) };\n'\
	'enum ternary Test_Result[_function_count_] = { ternary_0 };  /* Neither pass (+1) nor fail (-1): indicates test result is being determined */\n'\
	'char * c function_name[_function_count_] = { $(patsubst %,"%"$(comma),$(function_names)) };\n'\
	'\n'\
	'int main(int c Parameters, char c * c Parameter[]) {\n'\
	'	/* 0hana_test_dispatch start notice */\n'\
	'	fprintf(stderr, "[Test Dispatch]: ");\n'\
	'	if(Parameters > 1) {\n'\
	'		fprintf(stderr, "%%i test%%s required.\\n\\n  ", Parameters - 1, Parameters == 2 ? "" : "s");\n'\
	'	}\n'\
	'	else {\n'\
	'		fprintf(stderr, "No testing required.\\n");\n'\
	'		fprintf(stderr, "[Leak Analysis]:\\nPROCESS ID ");\n'\
	'		return 0;\n'\
	'	}\n'\
	'	for(int X = 1; X < Parameters; X++)\n'\
	'		{ fprintf(stderr, "%%s  ", Parameter[X]); }\n'\
	'	fprintf(stderr, "\\n\\n[Test Dispatch]: Execute.\\n\\n  ...");\n'\
	'	/* Required function calculation */\n'\
	'	for(int X = 0, Y = 1; X < _function_count_; X++) {\n'\
	'		if(Y < Parameters && strcmp(function_name[X],Parameter[Y]) == 0) {\n'\
	'			Y++;\n'\
	'		}\n'\
	'		else {\n'\
	'			Test_Result[X] = ternary_1;  /* Test not required -> implicit pass */\n'\
	'		}\n'\
	'	}\n'\
	'	/* #pragma omp parallel for */\n'\
	'	for(int X = 0; X < $(function_count); X++) {\n'\
	'		if(Test_Result[X] == ternary_0) {\n'\
	'			FILE * Log_File = 0;\n'\
	'			test[X](a(Log_File));\n'\
	'			if(Log_File isnt NULL) {\n'\
	'				fclose(Log_File);\n'\
	'				Test_Result[X] = ternary_2;  /* Test returned a Log_File -> implies failure */\n'\
	'			}\n'\
	'			else {\n'\
	'				Test_Result[X] = ternary_1;  /* Test did not return a failure log -> implicit pass */\n'\
	'				fprintf(stderr, "\\b\\b\\b%%s test: passed\\n  ...", function_name[X]);\n'\
	'			}\n'\
	'		}\n'\
	'	}\n'\
	'	fprintf(stderr, "\\b\\b\\b   \\n[Leak Analysis]:\\nPROCESS ID ");\n'\
	'	return 0;\n'\
	'}'

clean:
	@echo '-     Removing : build'
	@rm -r build

-include $(object_targets:.o=.d)
