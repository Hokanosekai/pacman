# ifndef GHOST_H
# define GHOST_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "map.h"
#include "player.h"

#define GHOST_SPEED 1
#define GHOST_SIZE 32

typedef enum {
    GHOST_UP,
    GHOST_DOWN,
    GHOST_LEFT,
    GHOST_RIGHT,
    GHOST_NULL
} GhostDirection;

typedef struct {
  int x, y;
  int speed;
  int animation_frame;
  GhostDirection direction;
  SDL_Texture *sprite;
  bool moving;
  bool is_active;
} Ghost;

/**
 * @brief Create a new ghost
 * @param window The window to create the ghost in
 * @param x The x position of the ghost
 * @param y The y position of the ghost
 * @param ghost_number The number of the ghost
 * @return A pointer to the ghost
 */
Ghost *ghost_create(Window *window, int x, int y, int ghost_number);

/**
 * @brief Destroy a ghost
 * @param ghost The ghost to destroy
 */
void ghost_destroy(Ghost *ghost);

/**
 * @brief Update the ghost
 * @param ghost The ghost to update
 * @param window The window to update the ghost in
 */
void ghost_update(Ghost *ghost, Window *window);

/**
 * @brief Render the ghost
 * @param ghost The ghost to render
 * @param window The window to render the ghost in
 */
void ghost_render(Ghost *ghost, Window *window);

/**
 * @brief Move the ghost
 * @param ghost The ghost to move
 * @param window The window to move the ghost in
 * @param map The map to move the ghost in
 */
void ghost_move(Ghost *ghost, Window *window, Map *map);

/**
 * @brief Reset the ghost
 * @param ghost The ghost to reset
 */
void ghost_reset(Ghost *ghost);

/**
 * @brief Check if the ghost is colliding with the player
 * @param ghost The ghost to check
 * @param player The player to check
 * @return True if the ghost is colliding with the player, false otherwise
 */
bool ghost_check_collision(Ghost *ghost, Player *player);

/**
 * @brief Animate the ghost
 * @param ghost The ghost to animate
 * @param window The window to animate the ghost in
 */
void ghost_animation(Ghost *ghost, Window *window);

# endif