# Zero Hanami -- 0hana/Build/random/Makefile

DFLAGS = -Wall -Wextra -g
CFLAGS = $(DFLAGS) -c
BUILD   = 0hana/Build/random/

queue: $(BUILD)random.o

$(BUILD)random.o: random.c
	gcc $(CFLAGS) random.c -o $(BUILD)random.o
