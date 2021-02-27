# Zero Hanami -- 0hana/Build/graph/makefile
# Invoked inside of 0hana/Source/graph

DFLAGS = -Wall -Wextra -g
CFLAGS = $(DFLAGS) -c
BUILD  = 0hana/Build/graph
GRAPH  = $(BUILD)/graph_

graph: $(GRAPH)bfs.o $(GRAPH)copy.o $(GRAPH)dfs.o $(GRAPH)random.o $(GRAPH)transpose.o

$(GRAPH)bfs.o: graph_bfs.c
	gcc $(CFLAGS) graph_bfs.c -o $(GRAPH)bfs.o
	@touch $(BUILD)

$(GRAPH)copy.o: graph_copy.c
	gcc $(CFLAGS) graph_copy.c -o $(GRAPH)copy.o
	@touch $(BUILD)

$(GRAPH)dfs.o: graph_dfs.c
	gcc $(CFLAGS) graph_dfs.c -o $(GRAPH)dfs.o
	@touch $(BUILD)

$(GRAPH)random.o: graph_random.c
	gcc $(CFLAGS) graph_random.c -o $(GRAPH)random.o
	@touch $(BUILD)

$(GRAPH)transpose.o: graph_transpose.c
	gcc $(CFLAGS) graph_transpose.c -o $(GRAPH)transpose.o
	@touch $(BUILD)

