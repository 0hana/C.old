# Hanami Zero (C) 2021: makefile
# Sync build against source

# New source file detection
$(foreach file,$(shell 2>/dev/null find source -name '*.c'),$(shell if [ ! -f $(file:source/%.c=build/%.o) ]; then rm -f build/0hana_test_dispatch*; fi))
# Directory structure synchronization
$(foreach directory,$(shell 2>/dev/null find build -mindepth 1 -type d),$(shell if [ ! -d $(directory:build/%=source/%) ]; then rm -rf $(directory); fi))
# (Re)Moved source file detection
$(foreach file,$(shell 2>/dev/null find build  -name '*.o'),$(shell if [ ! -f $(file:build/%.o=source/%.c) ]; then rm -f build/0hana_test_dispatch* $(file) $(file:.o=.s) $(file:.o=.d) $(file:.o=.log); fi))

.PHONY: clean contained container default docker image run uncontained

# default target switch: make default [contained|uncontained]
$(if $(filter default,$(MAKECMDGOALS)),\
	$(if $(filter uncontained,$(MAKECMDGOALS)),\
		$(shell sed -i 's/^default := contained$$/default := uncontained/' makefile)$(error CHANGE NOTICE: default target set to uncontained),\
		$(if $(filter contained,$(MAKECMDGOALS)),\
			$(shell sed -i 's/^default := uncontained$$/default := contained/' makefile)$(error CHANGE NOTICE: default target set to contained)\
		)\
	)\
)

default := contained

default: $(default)

clean:
	@echo '-     Removing : build'
	@$(if $(filter uncontained,$(default)),if [ -d build ]; then rm -r build; fi)
	@$(if $(filter   contained,$(default)),if (2>/dev/null docker ps | grep -q hanami); then docker exec -w /home hanami sh -c 'rm -r build'; fi)

contained: docker
	@docker cp license.txt hanami:/home
	@docker cp makefile    hanami:/home
	@docker cp readme.md   hanami:/home
	@docker cp source      hanami:/home
	@docker exec -w /home  hanami sh -c 'make -j uncontained'

docker: container

container: image
	@if ! docker ps | grep -q 'hanami'; then docker run -dti --rm --name hanami hanami; fi

dependencies := coreutils gcc libc-dev make valgrind

image:
	@if ! docker images | grep -q 'hanami'; then printf 'FROM alpine:3\nRUN apk add $(dependencies) gdb tree' | docker build -t hanami -; fi

uncontained: run

function_updates = $(sort $(notdir $(basename $(shell find build -name '*.log'))))

run: build/0hana_test_dispatch
	@echo '    READY!'
	@valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -q -s $< $(function_updates)

object_targets  := $(patsubst source/%.c,build/%.o,$(shell find source -name '*.c' | sed 's/\(.*\)\//\1@/' | sort -t@ -k2 | sed 's/@/\//'))

build/0hana_test_dispatch: build/0hana_test_dispatch.c $(object_targets)
	@echo '+ Synthesizing : Dispatcher'
	@gcc -Wall -Wextra -Wpedantic -g -o $@ $^ -D test='test'

build/%.o: build/%.s
	@echo '-  Mechanizing :  $< -> $@'
	@gcc -g -c $< -o $@
	@echo > $(@:.o=.log) "Test incomplete"

build/%.s: source/%.c
	@echo '-  Translating : $< -> $@'
	@mkdir -p $(dir $@)
	@gcc -Wall -Wextra -Wpedantic -g -o $@ $< -D test='v test_$(notdir $(basename $<))(FILE * * c Log_File, char c extra_spacing[])' -D log_file='"$(@:.s=.log)"' -D function_name='"$(notdir $(basename $<))"' -S -MMD

function_names  := $(notdir $(basename $(object_targets)))

build/0hana_test_dispatch.sed: $(object_targets:.o=.s)
	@echo '-   Generating :  build/0hana_test_dispatch.sed'
	@echo $(function_names) | tr ' ' '\n' | cat -n | sed 's|^[\t ]*\([0-9]\+\)[\t ]\+\([_0-9A-Za-z]\+\)|s\/\2\/\1 - 1,/|' > build/0hana_test_dispatch.sed  # sed function name to id number

function_count  := $(words $(function_names))
         comma  := ,

build/0hana_test_dispatch.c: build/0hana_test_dispatch.sed
	@echo '-   Generating :  build/0hana_test_dispatch.c'
	@printf > build/0hana_test_dispatch.c \
	'/* Hanami Zero (C) 2021: 0hana_test_dispatch.c */\n'\
	'// Unit testing main function\n'\
	'$(foreach header,$(shell find source -name '*.h'),#include "../$(header)"\n)'\
	'$(foreach function,$(function_names),\nv test_$(function)(FILE * * c Log_File$(comma) char c extra_spacing[]);\n)'\
	'\n'\
	'#define _function_count_ $(function_count)\n'\
	'char c * c function_name[_function_count_] = { $(function_names:%="%"$(comma)) };\n'\
	'char c * c extra_spacing[_function_count_] = { $(shell max_length=0; for name in $(function_names); do new_length=$$(printf "$${name}" | wc -m); if [ $${new_length} -gt $${max_length} ]; then max_length=$${new_length}; fi; done; for name in $(function_names); do length=$$(printf "$${name}" | wc -m); printf "\"$$(seq $$(($${max_length} - $${length})) | tr -s [:digit:] ' ' | tr -d '\n')\", "; done)};\n'\
	'v     (* c          test[_function_count_])(FILE * * c Log_File, char c extra_spacing[]) = { $(function_names:%=test_%$(comma)) };\n'\
	'enum ternary Test_Result[_function_count_] = { ternary_0 };  /* Neither pass (+1) nor fail (-1): indicates test result is being determined */\n'\
	'\n'\
	'$(foreach function,\
	 $(object_targets:.o=.s),\
	\n$(shell dependency="$$(grep call $(function) | grep -vw $(notdir $(basename $(function))) | grep -ow $$(echo '$(function_names:%=%\|)' | tr -d ' ') | sed -f build/0hana_test_dispatch.sed)";\
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
	'int    Topolexigraphical[_function_count_] = { -1 };\n'\
	'\n'\
	'v topolex_recurse(int Function_ID, bool Visited[], int * Topolex_Iterator) {\n'\
	'	Visited[Function_ID] = true;\n'\
	'	for(int D = 0; D < dependencies[Function_ID]; D++)\n'\
	'		if(Visited[dependency[Function_ID][D]] == false) topolex_recurse(dependency[Function_ID][D], Visited, Topolex_Iterator);\n'\
	'\n'\
	'	Topolexigraphical[--(*Topolex_Iterator)] = Function_ID;\n'\
	'}\n'\
	'\n'\
	'v topolexigraphical(v) {\n'\
	'	bool Visited[_function_count_] = { false };\n'\
	'	int Topolex_Iterator = _function_count_;\n'\
	'	for(int Function_ID = 0; Function_ID < _function_count_; Function_ID++) {\n'\
	'		if(Visited[Function_ID] is false)\n'\
	'			topolex_recurse(Function_ID, Visited, a(Topolex_Iterator));\n'\
	'	}\n'\
	'}\n'\
	'\n'\
	'int main(int c Parameters, char c * c Parameter[]) {\n'\
	'	/* 0hana_test_dispatch start notice */\n'\
	'	fprintf(stderr, "[ Dispatcher ] : ");\n'\
	'	if(Parameters > 1) {\n'\
	'		fprintf(stderr, "%%i function update%%s.\\nRequired tests :\\n\\n", Parameters - 1, Parameters == 2 ? "" : "s");\n'\
	'	}\n'\
	'	else {\n'\
	'		fprintf(stderr, "No function updates.\\n");\n'\
	'		fprintf(stderr, "[ Leak Stats ] : ");\n'\
	'		return 0;\n'\
	'	}\n'\
	'\n'\
	'	/* Updated function calculation */\n'\
	'	for(int X = 0, Y = 1; X < _function_count_; X++) {\n'\
	'		if(Y < Parameters && strcmp(function_name[X],Parameter[Y]) == 0) {\n'\
	'			Y++;\n'\
	'		}\n'\
	'		else {\n'\
	'			Test_Result[X] = ternary_1;  /* function not updated -> test not required IF dependencies not updated */\n'\
	'		}\n'\
	'	}\n'\
	'\n'\
	'	/* Required testing calculation */\n'\
	'\n'\
	'	topolexigraphical();\n'\
	'\n'\
	'	for(int X = _function_count_ - 1; X + 1 > 0; X--) {\n'\
	'		if(Test_Result[Topolexigraphical[X]] is ternary_1 /* implicit pass */) {\n'\
	'			for(int Y = 0; Y < dependencies[Topolexigraphical[X]]; Y++) {\n'\
	'				if(Test_Result[dependency[Topolexigraphical[X]][Y]] is ternary_0 /* Undetermined */) Test_Result[Topolexigraphical[X]] = ternary_2;  /* Depends on undetermined status, and is thus undetermined itself */\n'\
	'			}\n'\
	'		}\n'\
	'	}\n'\
	'\n'\
	'	/* Required testing print calls */\n'\
	'	for(int X = _function_count_ - 1; X + 1 > 0; X--) {\n'\
	'		if(Test_Result[Topolexigraphical[X]] is ternary_0) fprintf(stderr, "  %%s%%s  [ updated ]\\n", extra_spacing[Topolexigraphical[X]], function_name[Topolexigraphical[X]]);\n'\
	'		else\n'\
	'		if(Test_Result[Topolexigraphical[X]] is ternary_2) {\n'\
	'			Test_Result[Topolexigraphical[X]] = ternary_0;\n'\
	'			fprintf(stderr, "  %%s%%s  [ depends on:", extra_spacing[Topolexigraphical[X]], function_name[Topolexigraphical[X]]);\n'\
	'			for(int Y = 0; Y < dependencies[Topolexigraphical[X]]; Y++) {\n'\
	'				if(Test_Result[dependency[Topolexigraphical[X]][Y]] is ternary_0) fprintf(stderr, " %%s,", function_name[dependency[Topolexigraphical[X]][Y]]);'\
	'			}\n'\
	'			fprintf(stderr, " ]\\n");\n'\
	'		}\n'\
	'	}\n'\
	'\n'\
	'	fprintf(stderr, "\\n[ Dispatcher ]: Execute.\\n\\n  ...");\n'\
	'	/* #pragma omp parallel for */\n'\
	'	for(int X = _function_count_ - 1; X + 1 > 0; X--) {\n'\
	'		if(Test_Result[Topolexigraphical[X]] == ternary_0) {\n'\
	'			FILE * Log_File = 0;\n'\
	'			test[Topolexigraphical[X]](a(Log_File), extra_spacing[Topolexigraphical[X]]);\n'\
	'			if(Log_File isnt NULL) {\n'\
	'				fclose(Log_File);\n'\
	'				Test_Result[Topolexigraphical[X]] = ternary_2;  /* Test returned a Log_File -> implies failure */\n'\
	'			}\n'\
	'			else {\n'\
	'				Test_Result[Topolexigraphical[X]] = ternary_1;  /* Test did not return a failure log -> implicit pass */\n'\
	'				fprintf(stderr, "\\b\\b\\b%%s%%s  [ test: passed ]\\n  ...", extra_spacing[Topolexigraphical[X]], function_name[Topolexigraphical[X]]);\n'\
	'			}\n'\
	'		}\n'\
	'	}\n'\
	'	fprintf(stderr, "\\b\\b\\b   \\n[ Leak Stats ] : ");\n'\
	'	return 0;\n'\
	'}'

-include $(object_targets:.o=.d)
