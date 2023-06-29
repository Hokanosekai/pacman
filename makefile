CC = gcc
CFLAGS = -g -Wall
CLIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm

SRC_DIR = ./src
BIN_DIR = ./bin
OUTPUT_NAME = pacman

OBJS = $(BIN_DIR)/main.o $(BIN_DIR)/bonus.o $(BIN_DIR)/game.o $(BIN_DIR)/window.o $(BIN_DIR)/player.o $(BIN_DIR)/map.o $(BIN_DIR)/ghost.o 

all: init pacman

init:
	mkdir -p $(BIN_DIR)

pacman: $(OBJS) 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(OUTPUT_NAME) $(OBJS) $(CLIBS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

clean:
			rm -f $(BIN_DIR)/*.o
			rm -f $(BIN_DIR)/pacman