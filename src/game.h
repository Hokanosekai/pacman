# ifndef GAME_H
# define GAME_H

#include "game_state.h"
#include "window.h"
#include "player.h"
#include "map.h"
#include "ghost.h"

#define GHOST_AMOUNT 4
#define START_BUTTON_ANIMATION_SPEED 2

#define NUMBER_OF_DOT 205
#define NUMBER_OF_POWER_PELLET 4

typedef struct {
    int width, height;
    int scale;
    int score;
    Window *window;
    GameState state;
    Player *player;
    Map *map;
    Ghost *ghosts[GHOST_AMOUNT];
    SDL_Texture *heart_texture;
    int start_button_animation_frame;
    int best_scores[5];
    int number_of_dot, number_of_power_pellet;
    int level;
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

/**
 * @brief Check collision between the player and the map
 * @param game Game
 */
void game_check_collision(Game *game);

/**
 * @brief Go to the next level
 * @param game Game
 */
void game_next_level(Game *game);

/**
 * @brief Reset the game
 * @param game Game
 */
void game_reset(Game *game);

/**
 * @brief Display the menu screen
 * @param game Game
 */
void game_state_menu_draw(Game *game);

/**
 * @brief Display the start button
 * @param game Game
 */
void display_start_button(Game *game);

/**
 * @brief Display the best scores
 * @param game Game
 */
void display_best_scores(Game *game);

/**
 * @brief Display the game screen
 * @param game Game
 */
void game_state_game_draw(Game *game);

/**
 * @brief Display the score
 * @param game Game
 */
void display_score(Game *game);

/**
 * @brief Display the lives
 * @param game Game
 */
void display_lives(Game *game);

/**
 * @brief Display the level
 * @param game Game
 */
void display_level(Game *game);

/**
 * @brief Display the game over screen
 * @param game Game
 */
void game_state_game_over_draw(Game *game);

/**
 * @brief Insert a score in the best scores
 * @param game Game
 * @param score Score
 */
void game_insert_score(Game *game, int score);

/**
 * @brief Save the best scores
 * @param game Game
 */
void game_save_best_scores(Game *game);

/**
 * @brief Load the best scores
 * @param game Game
 */
void game_load_best_scores(Game *game);

# endif