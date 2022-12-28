CC = gcc
CFLAGS = -g -Wall
CLIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm

DEBUG_DIR = ../debug
OUTPUT_NAME = pacman

OBJS = $(DEBUG_DIR)/main.o $(DEBUG_DIR)/game.o $(DEBUG_DIR)/window.o $(DEBUG_DIR)/player.o $(DEBUG_DIR)/map.o $(DEBUG_DIR)/ghost.o

all: pacman

pacman: $(OBJS) 
	$(CC) $(CFLAGS) -o $(DEBUG_DIR)/$(OUTPUT_NAME) $(OBJS) $(CLIBS)

$(DEBUG_DIR)/%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $(DEBUG_DIR)/$@ $(CLIBS)

clean:
			rm -f $(DEBUG_DIR)/*.o pacman