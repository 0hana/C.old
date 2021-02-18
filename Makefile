CFLAGS=-Wall -Wextra -g

CUnit/graph: CUnit/main.graph.c Build/graph.o Build/random.o
	gcc $(CFLAGS) -lcunit CUnit/main.graph.c Build/graph.o Build/random.o -o CUnit/graph

Build/graph.o: graph.c
	gcc $(CFLAGS) -c graph.c -o Build/graph.o

Build/random.o: random.c
	gcc $(CFLAGS) -c random.c -o Build/random.o

clean:
	rm Build/graph.o Build/random.o
