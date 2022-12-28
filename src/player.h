# ifndef PLAYER_H
# define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "window.h"
#include "map.h"

#define PLAYER_SPEED 2
#define PLAYER_SIZE 32

typedef enum {
  PLAYER_NULL,
  PLAYER_UP,
  PLAYER_DOWN,
  PLAYER_LEFT,
  PLAYER_RIGHT
} PlayerDirection;

typedef struct {
  int x, y;
  int speed;
  int animation_frame;
  int lives;
  SDL_Texture *sprite;
  PlayerDirection direction;
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
 * @param x Player x position
 * @param y Player y position
 * @return Player*
 */
Player *player_create(Window *window, int x, int y);

/**
 * @brief Update the Player object
 * @param player Player
 * @param game Game
 */
void player_update(Player *player, Window *window);

/**
 * @brief Draw the Player object
 * @param player Player
 * @param game Game
 */
void player_draw(Player *player, Window *window);

/**
 * @brief Reset the Player object
 * @param player Player
 */
void player_reset(Player *player);

/**
 * @brief Destroy the Player object
 * @param player Player
 */
void player_destroy(Player *player);

# endif