CC = gcc
CFLAGS = -g -Wall
CLIBS = -lSDL2main -lSDL2 -lSDL2_image

DEBUG_DIR = ../debug
OUTPUT_NAME = pacman

OBJS = main.o window.o game.o

all: pacman

pacman: $(OBJS) 
	$(CC) $(CFLAGS) -o $(DEBUG_DIR)/$(OUTPUT_NAME) $(OBJS) $(CLIBS)

$(DEBUG_DIR)/%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $(DEBUG_DIR)/$@ $(CLIBS)
	cp $@ $(DEBUG_DIR)

clean:
			rm -f $(DEBUG_DIR)/*.o pacman