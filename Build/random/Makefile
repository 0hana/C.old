# Zero Hanami -- 0hana/Build/random/Makefile

DFLAGS = -Wall -Wextra -g
CFLAGS = $(DFLAGS) -c
BUILD  = 0hana/Build/random
RANDOM = $(BUILD)/random

queue: $(RANDOM).o

$(RANDOM).o: random.c
	gcc $(CFLAGS) random.c -o $(RANDOM).o
	@touch $(BUILD)
