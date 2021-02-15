CFLAGS=-Wall -Wextra -g

CUnit/graph: CUnit/graph.main.c graph.o random.o
	gcc $(CFLAGS) -lcunit CUnit/graph.main.c graph.o random.o -o CUnit/graph

graph.o: graph.c
	gcc $(CFLAGS) -c graph.c

random.o: random.c
	gcc $(CFLAGS) -c random.c

clean:
	rm graph.o random.o
