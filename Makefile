# Zero Hanami -- 0hana.C Makefile

CFLAGS=-Wall -Wextra -g

Docker:
	docker cp . hanami:/home
	docker exec -w /home hanami make Init

Init:
	test -e Artifact || mkdir Artifact
	test -d Artifact || ( rm Artifact && mkdir Artifact )
	test -e Build || mkdir Build
	test -d Build || ( rm Build && mkdir Build )
	make Build/test

Build/test: CUnit/test.main.c Build/graph.o Build/queue.o Build/random.o
	gcc $(CFLAGS) CUnit/test.main.c Build/graph.o Build/queue.o Build/random.o -o Build/test -lcunit
	@echo Make complete. Initiating CUnit via Valgrind
	@bash -c "valgrind --leak-check=yes Build/test 2> >(tee Artifact/test.err) 1> >(tee Artifact/test.out)"

Build/graph.o: graph.c
	gcc $(CFLAGS) -c graph.c -o Build/graph.o

Build/queue.o: queue.c
	gcc $(CFLAGS) -c queue.c -o Build/queue.o

Build/random.o: random.c
	gcc $(CFLAGS) -c random.c -o Build/random.o

clean:
	rm -rf Artifact Build
