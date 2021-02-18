CFLAGS=-Wall -Wextra -g

Build/test: CUnit/test.main.c Build/graph.o Build/queue.o Build/random.o
	gcc $(CFLAGS) -lcunit CUnit/test.main.c Build/graph.o Build/random.o -o Build/test
	@echo Make complete. Initiating CUnit
	@Build/test | tee Artifacts/test.out

Build/graph.o: graph.c
	gcc $(CFLAGS) -c graph.c -o Build/graph.o

Build/queue.o: queue.c
	gcc $(CFLAGS) -c queue.c -o Build/queue.o

Build/random.o: random.c
	gcc $(CFLAGS) -c random.c -o Build/random.o

clean:
	rm Build/test Build/graph.o Build/queue.o Build/random.o
