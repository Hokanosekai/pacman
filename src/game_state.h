# ifndef GAME_STATE_H
# define GAME_STATE_H

typedef enum {
    STATE_NULL = 0,
    STATE_MENU = 1,
    STATE_GAME = 2,
    STATE_PAUSE = 3,
    STATE_GAME_OVER = 4,
    STATE_EXIT = 5
} GameState;

# endif