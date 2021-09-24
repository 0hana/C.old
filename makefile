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

function_updates = $(sort $(notdir $(basename $(shell find build -name '*.log'))))

run: build/0hana_test_dispatch
	@echo "-    Executing : $<"
	@valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -q -s $< $(function_updates)

object_targets := $(patsubst source/%.c,build/%.o,$(shell find source -name '*.c' | sed 's/\(.*\)\//\1@/' | sort -t@ -k2 | sed 's/@/\//'))

build/0hana_test_dispatch: build/0hana_test_dispatch.c $(object_targets)
	@echo "- Synthesizing : $@"
	@gcc -Wall -Wextra -Wpedantic -g -o $@ $^ -D test='test'

build/%.o: build/%.s
	@echo "-  Translating :  $< -> $@"
	@gcc -g -c $< -o $@
	@echo > $(@:.o=.log) "Test incomplete"

.SECONDARY: $(object_targets:.o=.s)

build/%.s: source/%.c
	@echo '-  Translating : $< -> $@'
	@gcc -Wall -Wextra -Wpedantic -g -o $@ $< -D test='v test_$(notdir $(basename $<))(FILE * * c Log_File, char c extra_spacing[])' -D log_file='"$(@:.s=.log)"' -D function_name='"$(notdir $(basename $<))"' -S -MMD

function_names := $(notdir $(basename $(object_targets)))
function_count := $(words $(function_names))
         comma := ,

build/0hana_test_dispatch.c: build/0hana-sed-fn2idn  # sed function names to id numbers
	@echo "-   Generating : $@"
	@printf > $@ \
	'/* Hanami Zero (C) 2021: 0hana_test_dispatch.c */\n'\
	'// Unit testing main function\n'\
	'$(patsubst %,\n#include "../%",$(shell find source -name '*.h'))\n'\
	'$(patsubst %,\nv test_%(FILE * * c Log_File, char c extra_spacing[]);,$(function_names))\n'\
	'\n'\
	'#define _function_count_ $(function_count)\n'\
	'v     (* c          test[_function_count_])(FILE * * c Log_File, char c extra_spacing[]) = { $(patsubst %,test_%$(comma),$(function_names)) };\n'\
	'enum ternary Test_Result[_function_count_] = { ternary_0 };  /* Neither pass (+1) nor fail (-1): indicates test result is being determined */\n'\
	'\n'\
	'char c * c function_name[_function_count_] = { $(patsubst %,"%"$(comma),$(function_names)) };\n'\
	'char c * c extra_spacing[_function_count_] = { $(shell max_length=0; for name in $(function_names); do new_length=$$(printf "$${name}" | wc -m); if [ $${new_length} -gt $${max_length} ]; then max_length=$${new_length}; fi; done; for name in $(function_names); do length=$$(printf "$${name}" | wc -m); printf "\"$$(seq $$(($${max_length} - $${length})) | tr -s [:digit:] ' ' | tr -d '\n')\", "; done)};\n'\
	'$(foreach function,\
	$(object_targets:.o=.s),\
	\n$(shell dependency="$$(grep call $(function) | grep -vw $(notdir $(basename $(function))) | grep -ow $$(echo '$(function_names:%=%\|)' | tr -d ' ') | sed -f build/0hana-sed-fn2idn)";\
		if [ -z "$${dependency}" ];\
		then echo "#define       dependency_$(notdir $(basename $(function))) NULL";\
		else echo "int  c        dependency_$(notdir $(basename $(function)))[] = { $${dependency} };";\
		fi\
	))\n'\
	'\n'\
	'int  c * c    dependency[_function_count_] = {\n'\
	'$(foreach function,$(function_names),\tdependency_$(function),\n)'\
	'};\n'\
	'\n'\
	'int  c      dependencies[_function_count_] = {\n'\
	'$(foreach function,$(function_names),\t((z)dependency_$(function) + 0 ? s(dependency_$(function)) / s(int) : 0),\n)'\
	'};\n'\
	'\n'\
	'int main(int c Parameters, char c * c Parameter[]) {\n'\
	'	/* 0hana_test_dispatch start notice */\n'\
	'	fprintf(stderr, "[Test Dispatch]: ");\n'\
	'	if(Parameters > 1) {\n'\
	'		fprintf(stderr, "%%i function update%%s.\\n\\n", Parameters - 1, Parameters == 2 ? "" : "s");\n'\
	'	}\n'\
	'	else {\n'\
	'		fprintf(stderr, "No function updates.\\n");\n'\
	'		fprintf(stderr, "[Leak Analysis]: ");\n'\
	'		return 0;\n'\
	'	}\n'\
	'\n'\
	'	/* Required function calculation */\n'\
	'	for(int X = 0, Y = 1; X < _function_count_; X++) {\n'\
	'		if(Y < Parameters && strcmp(function_name[X],Parameter[Y]) == 0) {\n'\
	'			fprintf(stderr, "  %%s%%s  [ Depends on (%%i):", extra_spacing[X], function_name[X], dependencies[X]);\n'\
	'			for(int Z = 0; Z + 1 < dependencies[X]; Z++) {\n'\
	'				fprintf(stderr, " %%s,", function_name[dependency[X][Z]]);\n'\
	'			}\n'\
	'			if(dependencies[X]) fprintf(stderr, " %%s", function_name[dependency[X][dependencies[X] - 1]]);\n'\
	'			fprintf(stderr, " ]\\n");\n'\
	'			Y++;\n'\
	'		}\n'\
	'		else {\n'\
	'			Test_Result[X] = ternary_1;  /* Test not required -> implicit pass */\n'\
	'		}\n'\
	'	}\n'\
	'\n'\
	'	fprintf(stderr, "\\n[Test Dispatch]: Execute.\\n\\n  ...");\n'\
	'	/* #pragma omp parallel for */\n'\
	'	for(int X = 0; X < _function_count_; X++) {\n'\
	'		if(Test_Result[X] == ternary_0) {\n'\
	'			FILE * Log_File = 0;\n'\
	'			test[X](a(Log_File), extra_spacing[X]);\n'\
	'			if(Log_File isnt NULL) {\n'\
	'				fclose(Log_File);\n'\
	'				Test_Result[X] = ternary_2;  /* Test returned a Log_File -> implies failure */\n'\
	'			}\n'\
	'			else {\n'\
	'				Test_Result[X] = ternary_1;  /* Test did not return a failure log -> implicit pass */\n'\
	'				fprintf(stderr, "\\b\\b\\b%%s%%s  test: passed\\n  ...", extra_spacing[X], function_name[X]);\n'\
	'			}\n'\
	'		}\n'\
	'	}\n'\
	'	fprintf(stderr, "\\b\\b\\b   \\n[Leak Analysis]: ");\n'\
	'	return 0;\n'\
	'}'

build/0hana-sed-fn2idn: $(object_targets:.o=.s)
	@echo '-   Generating : build/0hana-sed-fn2idn'
	@echo $(function_names) | tr ' ' '\n' | cat -n | sed 's|^[\t ]*\([0-9]\+\)[\t ]\+\([_0-9A-Za-z]\+\)|s\/\2\/\1 - 1,/|' > build/0hana-sed-fn2idn  # sed function name to id number

clean:
	@echo '-     Removing : build'
	@$(if $(filter uncontained,$(MAKECMDGOALS)),rm -r build,docker exec -w /home hanami sh -c 'rm -r build')

-include $(object_targets:.o=.d)
