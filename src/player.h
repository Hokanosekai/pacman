# ifndef PLAYER_H
# define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"

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
  SDL_Texture *texture;
  PlayerDirection direction;
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
 * @brief Destroy the Player object
 * @param player Player
 */
void player_destroy(Player *player);

void player_check_collision(Player *player, Window *window);

# endif