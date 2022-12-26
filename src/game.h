# ifndef GAME_H
# define GAME_H

#include "game_state.h"
#include "window.h"

typedef struct {
    int width;
    int height;
    int scale;
    int running;
    Window *window;
    GameState *state;
} Game;

/**
 * @brief Create a Game object
 * @param width Game width
 * @param height Game height
 * @param scale Game scale
 * @return Game*
 */
Game *game_create(int width, int height, int scale);

/**
 * @brief Destroy the Game object
 * @param game Game
 */
void game_destroy(Game *game);

/**
 * @brief Run the game
 * @param game Game
 */
void game_run(Game *game);

void game_state_menu_draw(Game *game);

void game_state_game_draw(Game *game);

void game_state_game_over_draw(Game *game);

# endif