CFLAGS=-Wall -Wextra -g

CUnit/graph: CUnit/main.graph.c graph.o random.o
	gcc $(CFLAGS) -lcunit CUnit/main.graph.c graph.o random.o -o CUnit/graph

graph.o: graph.c
	gcc $(CFLAGS) -c graph.c

random.o: random.c
	gcc $(CFLAGS) -c random.c

clean:
	rm graph.o random.o
