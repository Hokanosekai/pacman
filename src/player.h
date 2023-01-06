# ifndef PLAYER_H
# define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "window.h"
#include "map.h"

#define PLAYER_SPEED 4
#define PLAYER_SIZE 32

#define PLAYER_SPAWN_X 17
#define PLAYER_SPAWN_Y 22

#define PLAYER_ANIMATION_SPEED 3
#define PLAYER_INVINCIBLE_TIME 300
#define PLAYER_LIVES 3
#define PLAYER_FRAMES 6

typedef enum {
  PLAYER_NULL,
  PLAYER_UP,
  PLAYER_DOWN,
  PLAYER_LEFT,
  PLAYER_RIGHT
} PlayerDirection;

typedef struct {
  int x, y;
  int next_x, next_y;
  int speed, speed_timer;
  int animation_frame, animation_timer;
  int lives;
  SDL_Texture *sprite;
  PlayerDirection direction, next_direction;
  bool moving;
  bool dead;
  bool invincible;
  int invincible_timer;
  int number_of_dots_eaten;
  int number_of_power_pellets_eaten;
  int number_of_ghosts_eaten;
} Player;

/**
 * @brief Create a Player object
 * @param window Window
 * @return Player*
 */
Player *player_create(Window *window);

/**
 * @brief Update the Player object
 * @param map Map
 * @param player Player
 * @param key SDL_KeyboardEvent
 */
void player_update(Map *map, Player *player, SDL_KeyboardEvent key);

/**
 * @brief Move the Player object
 * @param player Player
 */
void player_move(Player *player);

/**
 * @brief Move the Player object to spawn
 * @param player Player
 */
void player_move_to_spawn(Player *player);

/**
 * @brief Draw the Player object
 * @param player Player
 * @param game Game
 */
void player_render(Player *player, Window *window);

/**
 * @brief Kill the Player object
 * @param player Player
 */
void player_kill(Player *player);

/**
 * @brief Reset the Player object
 * @param player Player
 */
void player_reset(Player *player);

/**
 * @brief Reset the Player lives
 * @param player Player
 */
void player_reset_lives(Player *player);

/**
 * @brief Destroy the Player object
 * @param player Player
 */
void player_destroy(Player *player);

/**
 * @brief Set the Player direction
 * @param player Player
 * @param direction PlayerDirection
 */
void player_set_direction(Player *player, PlayerDirection direction);

# endif