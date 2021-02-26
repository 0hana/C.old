# Zero Hanami -- 0hana.C Makefile

DFLAGS=-Wall -Wextra -g
CFLAGS=$(DFLAGS) -c

Docker:
	docker cp . hanami:/home
	docker exec -w /home hanami make Init

Init:
	test -e Artifact || mkdir Artifact
	test -d Artifact || ( rm Artifact && mkdir Artifact )
	test -e Build || mkdir Build
	test -d Build || ( rm Build && mkdir Build )
	make Build/test

Build/test: Build/test.main.o Build/test.graph.o Build/graph.o Build/queue.o Build/random.o
	gcc $(DFLAGS) Build/test.main.o Build/test.graph.o Build/graph.o Build/queue.o Build/random.o -o Build/test -lcunit
	@echo Make complete. Initiating CUnit via Valgrind
	@bash -c "valgrind --leak-check=full --show-leak-kinds=all Build/test 2> >(tee Artifact/test.err) 1> >(tee Artifact/test.out)"

Build/test.main.o: CUnit/test.main.c
	gcc $(CFLAGS) CUnit/test.main.c -o Build/test.main.o

Build/test.graph.o: CUnit/test.graph.c
	gcc $(CFLAGS) CUnit/test.graph.c -o Build/test.graph.o

Build/graph.o: Source/graph.c
	gcc $(CFLAGS) Source/graph.c -o Build/graph.o

Build/queue.o: Source/queue.c
	gcc $(CFLAGS) Source/queue.c -o Build/queue.o

Build/random.o: Source/random.c
	gcc $(CFLAGS) Source/random.c -o Build/random.o

re-run:
	docker exec -w /home hanami valgrind --leak-check=full --show-leak-kinds=all Build/test

clean:
	rm -rf Artifact Build
