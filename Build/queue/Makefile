# Zero Hanami -- 0hana/Build/queue/Makefile

DFLAGS = -Wall -Wextra -g
CFLAGS = $(DFLAGS) -c
BUILD   = 0hana/Build/queue/

queue: $(BUILD)queue.o

$(BUILD)queue.o: queue.c
	gcc $(CFLAGS) queue.c -o $(BUILD)queue.o
