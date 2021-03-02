# Zero Hanami -- 0hana/C Makefile
# Invoked inside 0hana

PARALLEL = # Set a job limit for parallel compilations, default is unlimited -- Do note recursion in this file

DFLAGS = -Wall -Wextra -g
CFLAGS = $(DFLAGS) -c
OBJECT = 0hana/Build

GRAPH  = $(OBJECT)/graph/graph_
GRAPH_OBJECTS  = $(GRAPH)bfs.o $(GRAPH)copy.o $(GRAPH)dfs.o $(GRAPH)random.o $(GRAPH)transpose.o

QUEUE  = $(OBJECT)/queue/
QUEUE_OBJECTS  = $(QUEUE)queue.o

RANDOM = $(OBJECT)/random/
RANDOM_OBJECTS = $(RANDOM)random.o


Docker:
	@if ! (id | awk '{ print $3 }' | grep "(docker)" >/dev/null); then echo >&2 "You must be a member of the docker group to use the container system"; fi
	@docker cp . hanami:/home
	@docker exec -w /home hanami make Init

docker-image:
	@if ! (id | awk '{ print $3 }' | grep "(docker)" >/dev/null); then echo >&2 "You must be a member of the docker group to use the container system"; fi
	@printf "FROM alpine:3 as compile\nRUN apk add bash coreutils cunit cunit-dev gcc libc-dev make valgrind\n" | docker build -t hanami -

container:
	@if ! (id | awk '{ print $3 }' | grep "(docker)" >/dev/null); then echo >&2 "You must be a member of the docker group to use the container system"; fi
	@docker run -dti --rm --name hanami hanami

# Init assumes filesystem keeps track of change time in nanoseconds (and that at least 1 nanosecond passes before any recipes are built) -- this is a dependency of communication between build system and testing facility to minimize the number of tests that need to be run

Init:
	@touch Build # Save starting time of the build for comparison with object files to be generated -- access with $$(date -d "$$(stat -c %z Build)" +%s%N) -- $$ escapes makefile variable to use shell variables
	@sleep 0.001 # With gnu coreutils installed (see line 2 of Dockerfile), this should be guaranteed to pause make long enough to ensure it can correctly change timestamp object files to be generated
	test -e Artifact || mkdir Artifact
	if [ -d Artifact ]; then if [ -e Artifact/source-updates.txt ]; then rm Artifact/source-updates.txt; fi; else rm Artifact && mkdir Artifact; fi
	make -j $(PARALLEL) graph queue random
	@cd $(OBJECT); pwd; for DIRECTORY in *; do if [ -d $${DIRECTORY} ] && [ $$(echo $$(date -d "$$(stat -c %z ../Build)" +%s%N) - $$(date -d "$$(stat -c %z $${DIRECTORY})" +%s%N) | bc) -lt 0 ] ; then cd $${DIRECTORY}; for OBJECT in *; do if [ $$(echo $$(date -d "$$(stat -c %z ../../Build)" +%s%N) - $$(date -d "$$(stat -c %z $${OBJECT})" +%s%N) | bc) -lt 0 ]; then echo "$${OBJECT:0:$${#OBJECT}-2} " | tr -d '\n' >> ../../Artifact/source-updates.txt; fi; done; cd ..; fi; done; echo >> ../Artifact/source-updates.txt
	make -j $(PARALLEL) Build/test

Build/test: Build/test.main.o Build/test.graph.o $(GRAPH_OBJECTS) $(QUEUE_OBJECTS) $(RANDOM_OBJECTS)
	gcc $(DFLAGS) Build/test.main.o Build/test.graph.o $(GRAPH_OBJECTS) $(QUEUE_OBJECTS) $(RANDOM_OBJECTS) -o Build/test -lcunit
	@echo Make complete. Initiating CUnit via Valgrind
	@bash -c "valgrind --leak-check=full --show-leak-kinds=all Build/test $$(cat Artifact/source-updates.txt) 2> >(tee Artifact/test.err) 1> >(tee Artifact/test.out)"
	@cp Artifact/source-updates.txt Artifact/source-updates.old

Build/test.main.o: CUnit/test.main.c
	gcc $(CFLAGS) CUnit/test.main.c -o Build/test.main.o

Build/test.graph.o: CUnit/test.graph.c
	gcc $(CFLAGS) CUnit/test.graph.c -o Build/test.graph.o

graph:
	make -j $(PARALLEL) -C Source/graph -f 0hana/Build/graph/Makefile
	@echo

queue:
	make -j $(PARALLEL) -C Source/queue -f 0hana/Build/queue/Makefile
	@echo

random:
	make -j $(PARALLEL) -C Source/random -f 0hana/Build/random/Makefile
	@echo

re-run:
	docker exec -w /home hanami sh -c "valgrind --leak-check=full --show-leak-kinds=all Build/test \$$(cat Artifact/source-updates.old)"

report:
	@docker exec -w /home/Artifact hanami stat -c %z test.err
	@echo
	@docker exec -w /home/Artifact hanami cat test.err
	@echo
	@docker exec -w /home/Artifact hanami stat -c %z test.out
	@docker exec -w /home/Artifact hanami cat test.out

clean:
	docker exec -w /home hanami rm -rf *

utilities:
	@if ! (id | awk '{ print $3 }' | grep "(docker)" >/dev/null); then echo >&2 "You must be a member of the docker group to use the container system"; fi
