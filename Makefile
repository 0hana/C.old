# Zero Hanami -- 0hana/C Makefile

PARALLEL = # Set a job limit for parallel compilations, default is unlimited -- Do note recursion in this file

DFLAGS = -Wall -Wextra -g
CFLAGS = $(DFLAGS) -c
OBJECT = 0hana/Build

GRAPH  = $(OBJECT)/graph/graph_
GRAPH_OBJECTS  = $(GRAPH)bfs.o $(GRAPH)dfs.o $(GRAPH)free.o $(GRAPH)random.o $(GRAPH)transpose.o

QUEUE  = $(OBJECT)/queue/
QUEUE_OBJECTS  = $(QUEUE)queue.o

RANDOM = $(OBJECT)/random/
RANDOM_OBJECTS = $(RANDOM)random.o

Docker:
	docker cp . hanami:/home
	docker exec -w /home hanami make Init

Init:
	test -e Artifact || mkdir Artifact
	test -d Artifact || ( rm Artifact && mkdir Artifact )
	test -e Build || mkdir Build
	test -d Build || ( rm Build && mkdir Build )
	make -j $(PARALLEL) graph queue random
	make Build/test

Build/test: Build/test.main.o Build/test.graph.o $(GRAPH_OBJECTS) $(QUEUE_OBJECTS) $(RANDOM_OBJECTS)
	gcc $(DFLAGS) Build/test.main.o Build/test.graph.o $(GRAPH_OBJECTS) $(QUEUE_OBJECTS) $(RANDOM_OBJECTS) -o Build/test -lcunit
	@echo Make complete. Initiating CUnit via Valgrind
	@bash -c "valgrind --leak-check=full --show-leak-kinds=all Build/test 2> >(tee Artifact/test.err) 1> >(tee Artifact/test.out)"

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
	docker exec -w /home hanami valgrind --leak-check=full --show-leak-kinds=all Build/test

clean:
	docker exec -w /home hanami rm -rf Artifact Build
