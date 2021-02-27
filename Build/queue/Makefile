# Zero Hanami -- 0hana/Build/queue/Makefile

DFLAGS = -Wall -Wextra -g
CFLAGS = $(DFLAGS) -c
BUILD  = 0hana/Build/queue
QUEUE  = $(BUILD)/queue

queue: $(QUEUE).o

$(QUEUE).o: queue.c
	gcc $(CFLAGS) queue.c -o $(QUEUE).o
	@touch $(BUILD)
