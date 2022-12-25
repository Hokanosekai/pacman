CC = gcc
CFLAGS = -g -Wall

DEBUG_DIR = ../debug
OUTPUT_NAME = pacman

OBJS = main.o

all: pacman

pacman: $(OBJS)
	$(CC) $(CFLAGS) -o $(DEBUG_DIR)/$(OUTPUT_NAME) $(DEBUG_DIR)/$(OBJS)

main.o: main.c
		$(CC) $(CFLAGS) -c main.c -o $(DEBUG_DIR)/main.o

clean:
			rm -f $(DEBUG_DIR)/*.o pacman