CFLAGS=-Wall -Wextra -g

Init:
	test -e Artifact || mkdir Artifact
	test -d Artifact || ( rm Artifact && mkdir Artifact )
	test -e Build || mkdir Build
	test -d Build || ( rm Build && mkdir Build )
	make Build/test

Build/test: CUnit/test.main.c Build/graph.o Build/queue.o Build/random.o
	gcc $(CFLAGS) -lcunit CUnit/test.main.c Build/graph.o Build/queue.o Build/random.o -o Build/test
	@echo Make complete. Initiating CUnit via Valgrind
	@valgrind --leak-check=yes Build/test 2> >(tee Artifact/test.err) 1> >(tee Artifact/test.out)
	@echo "See results in Artifacts/test.err and Artifacts/test.out"
# Should add a clear and simple "All good" indicator if everything went well, else a clear "Investigate errors"
# Would have been helpful for me in my last internship. Maybe not much, but a little reassurance of what I was looking at,
# particularly if things were good to go

Build/graph.o: graph.c
	gcc $(CFLAGS) -c graph.c -o Build/graph.o

Build/queue.o: queue.c
	gcc $(CFLAGS) -c queue.c -o Build/queue.o

Build/random.o: random.c
	gcc $(CFLAGS) -c random.c -o Build/random.o

clean:
	rm -rf Build
