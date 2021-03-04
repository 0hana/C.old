# Zero Hanami -- 0hana/C Makefile
# Invoked inside 0hana

DFLAGS := -Wall -Wextra -g
CFLAGS := $(DFLAGS) -c
MAKE_PHASE := PHASE
PARALLEL :=
.PHONY: update autonomous docker container utilities

update:
	@if ! (id | awk '{ print $3 }' | grep "(docker)" >/dev/null); then echo >&2 "You must be a member of the docker group to use the container system"; fi
	@docker cp . hanami:/home
	@docker exec -w /home hanami make autonomous

autonomous:
	@\
	if [ -e Build ]; then\
		if [ -d Build ]; then\
			touch Build;\
		else\
			rm Build;\
			mkdir Build;\
		fi;\
	else\
		mkdir Build;\
	fi;
	@sleep 0.001;
	@test -e Artifact || mkdir Artifact
	@if [ -d Artifact ]; then if [ -e Artifact/source-updates.txt ]; then rm Artifact/source-updates.txt; fi; else rm Artifact && mkdir Artifact; fi
	@\
	DIRECTORIES=$$(find Source -type d -print | sed s/Source/Build/);\
	TARGETS=$$(find Source -type f -print | grep .c$$ | sed 's/.c$$/.o/;s/Source/Build/');\
	for PRESENT in $$(find Build -type d); do\
		REMOVE=1;\
		for CORRECT in $${DIRECTORIES}; do\
			if [ $${PRESENT} == $${CORRECT} ]; then\
				REMOVE=0;\
				break;\
			fi;\
		done;\
		if [ $${REMOVE} -eq 1 ]; then rm -r $${PRESENT}; fi;\
	done;\
	mkdir -p $${DIRECTORIES};\
	for OBJECT in $$(find Build -type f); do\
		REMOVE=1;\
		for CORRECT in $${TARGETS}; do\
			if [ $${OBJECT} == $${CORRECT} ]; then\
				REMOVE=0;\
				break;\
			fi;\
		done;\
		if [ $${REMOVE} -eq 1 ]; then rm -r $${OBJECT}; fi;\
	done;\
	printf "\n$(MAKE_PHASE): $$(echo $${TARGETS} | wc -w) object compilation ...\n\n";\
	time -f "%E" make -j $(PARALLEL) $${TARGETS};
	@\
	cd Build;\
	pwd;\
	for DIRECTORY in *; do\
		if [ -d $${DIRECTORY} ] && [ $$(echo $$(date -d "$$(stat -c %z ../Build)" +%s%N) - $$(date -d "$$(stat -c %z $${DIRECTORY})" +%s%N) | bc) -lt 0 ] ; then\
			cd $${DIRECTORY};\
			for OBJECT in *; do\
				if [ $$(echo $$(date -d "$$(stat -c %z ../../Build)" +%s%N) - $$(date -d "$$(stat -c %z $${OBJECT})" +%s%N) | bc) -lt 0 ]; then\
					echo "$${OBJECT:0:$${#OBJECT}-2} " | tr -d '\n' >> ../../Artifact/source-updates.txt;\
				fi;\
			done;\
			cd ..;\
		fi;\
	done;\
	echo >> ../Artifact/source-updates.txt
	@make Build/CUnit/main

docker:
	@if ! (id | awk '{ print $3 }' | grep "(docker)" >/dev/null); then echo >&2 "You must be a member of the docker group to use the container system"; fi
	@printf "FROM alpine:3 as compile\nRUN apk add bash coreutils cunit cunit-dev gcc libc-dev make tree valgrind\n" | docker build -t hanami -

container:
	@if ! (id | awk '{ print $3 }' | grep "(docker)" >/dev/null); then echo >&2 "You must be a member of the docker group to use the container system"; fi
	@docker run -dti --rm --name hanami hanami

utilities:
	@if ! (id | awk '{ print $3 }' | grep "(docker)" >/dev/null); then echo >&2 "You must be a member of the docker group to use the container system"; fi

Build/%.o: Source/%.c
	gcc $(CFLAGS) $? -o $@

Build/CUnit/main:
	@printf "\n$(MAKE_PHASE): Final executable linking ...\n\n"
	@time -f "%E" gcc $(DFLAGS) $$(find Build -type f -name *.o) -o $@ -lcunit;
	@echo Make complete. Initiating CUnit via Valgrind
	@bash -c "valgrind --leak-check=full --show-leak-kinds=all Build/CUnit/main $$(cat Artifact/source-updates.txt) 2> >(tee Artifact/test.err) 1> >(tee Artifact/test.out)"
	@cp Artifact/source-updates.txt Artifact/source-updates.old

re-run:
	docker exec -w /home hanami sh -c "valgrind --leak-check=full --show-leak-kinds=all Build/CUnit/main \$$(cat Artifact/source-updates.old)"

report:
	@docker exec -w /home/Artifact hanami stat -c %z test.err
	@echo
	@docker exec -w /home/Artifact hanami cat test.err
	@echo
	@docker exec -w /home/Artifact hanami stat -c %z test.out
	@docker exec -w /home/Artifact hanami cat test.out

clean:
	docker exec -w /home hanami sh -c 'rm -rf *'
