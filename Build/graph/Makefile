# Zero Hanami -- 0hana/Build/graph/makefile

DFLAGS = -Wall -Wextra -g
CFLAGS = $(DFLAGS) -c
BUILD   = 0hana/Build/graph/graph_

graph: $(BUILD)bfs.o $(BUILD)dfs.o $(BUILD)free.o $(BUILD)random.o $(BUILD)transpose.o

$(BUILD)bfs.o: graph_bfs.c
	gcc $(CFLAGS) graph_bfs.c -o $(BUILD)bfs.o

$(BUILD)dfs.o: graph_dfs.c
	gcc $(CFLAGS) graph_dfs.c -o $(BUILD)dfs.o

$(BUILD)free.o: graph_free.c
	gcc $(CFLAGS) graph_free.c -o $(BUILD)free.o

$(BUILD)random.o: graph_random.c
	gcc $(CFLAGS) graph_random.c -o $(BUILD)random.o

$(BUILD)transpose.o: graph_transpose.c
	gcc $(CFLAGS) graph_transpose.c -o $(BUILD)transpose.o

