# ifndef GAME_H
# define GAME_H

#include <stdbool.h>

#include "bonus.h"
#include "game_state.h"
#include "window.h"
#include "player.h"
#include "map.h"
#include "ghost.h"

#define GHOST_AMOUNT 4
#define START_BUTTON_ANIMATION_SPEED 20

#define NUMBER_OF_DOT 205
#define NUMBER_OF_POWER_PELLET 4

#define SCORE_FILE "../data/scores.txt"
#define LEVEL_FILE "../data/level.txt"

#define FONT_FILE "../assets/fonts/font.ttf"

#define DEFAULT_FONT_SIZE 20
#define GAME_OVER_FONT_SIZE 50
#define INSERT_COIN_FONT_SIZE 40
#define FPS_FONT_SIZE 30

#define MAP_TEXTURE_FILE "../assets/textures/tileset.png"

#define PLAYER_TEXTURE_FILE "../assets/sprites/pacman2.png"
#define GHOST_TEXTURE_FILE "../assets/sprites/ghost_%d.png"
#define GHOST_SCARED_TEXTURE_FILE "../assets/sprites/ghost_scared.png"

#define HEART_TEXTURE_FILE "../assets/sprites/heart.png"

#define KEYS_NUMBER 322
#define PRESS_KEY_DELAY 0.001f

#define PSEUDO_MAX_LENGTH 10

typedef struct {
    int width, height;
    int scale;
    int score, level;
    Window *window;
    GameState state;
    Player *player;
    Map *map;
    Ghost *ghosts[GHOST_AMOUNT];
    SDL_Texture *heart_texture;
    Bonus *bonus;
    bool is_paused, is_key_pressed;
    int start_button_animation_frame;
    char **best_scores;
    char *pseudo;
    int pseudo_index;
    int number_of_dot, number_of_power_pellet;
    SDL_KeyboardEvent last_key;
    const Uint8 *keys;
    float key_press_timer;
    bool display_fps;
    int fps;
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
 * @brief Update the game
 * @param game Game
 * @param delta_time Delta time
 */
void game_update(Game *game, float delta_time);

/**
 * @brief Render the game
 * @param game Game
 */
void game_render(Game *game);

/**
 * @brief Game input
 * @param game Game
 */
void game_input(Game *game);

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
 * @brief Display the FPS
 * @param game Game
 */
void display_fps(Game *game);

/**
 * @brief Display the menu screen
 * @param game Game
 */
void game_state_menu_draw(Game *game);

/**
 * @brief Update the menu screen
 * @param game Game
 * @param delta_time Delta time
 */
void game_state_menu_update(Game *game, float delta_time);

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
 * @brief Update the game screen
 * @param game Game
 * @param delta_time Delta time
 */
void game_state_game_update(Game *game, float delta_time);

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
 * @brief Display the pause screen
 * @param game Game
 */
void display_pause(Game *game);

/**
 * @brief Display the game over screen
 * @param game Game
 */
void game_state_game_over_draw(Game *game);

/**
 * @brief Update the game over screen
 * @param game Game
 * @param delta_time Delta time
 */
void game_state_game_over_update(Game *game, float delta_time);

/**
 * @brief Display the game over text
 * @param game Game
 */
void display_game_over(Game *game);

/**
 * @brief Display the insert name field
 * @param game Game
 */
void display_insert_name(Game *game);

/**
 * @brief Insert a score in the best scores
 * @param game Game
 * @param score Score
 * @param pseudo Pseudo
 */
void game_insert_score(Game *game, int score, char *pseudo);

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

/**
 * @brief Global input
 * @param game Game
 * @param key Keyboard key
 */
void game_global_input(Game *game, SDL_KeyboardEvent key);

# endif