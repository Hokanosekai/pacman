# ifndef GHOST_H
# define GHOST_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "map.h"
#include "player.h"

#define GHOST_SPEED 4
#define GHOST_SIZE 32

#define GHOST_SPAWN_X 18
#define GHOST_SPAWN_Y 11

#define GHOST_ANIMATION_SPEED 10
#define GHOST_FRAMES 6

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
  int animation_frame, animation_timer;
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
 * @param map The map to update the ghost in
 * @param ghost The ghost to update
 * @param player The player to update the ghost towards
 */
void ghost_update(Map *map, Ghost *ghost, Player *player);

/**
 * @brief Render the ghost
 * @param ghost The ghost to render
 * @param window The window to render the ghost in
 */
void ghost_render(Ghost *ghost, Window *window);

/**
 * @brief Move the ghost
 * @param ghost The ghost to move
 * @param player The player to move the ghost towards
 * @param map The map to move the ghost in
 */
void ghost_move(Ghost *ghost, Player *player, Map *map);

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

/**
 * @brief Get the direction of the ghost
 * @param map The map to get the direction in
 * @param ghost The ghost to get the direction of
 * @param player The player to get the direction towards
 * @return The direction of the ghost
 */
GhostDirection ghost_get_direction(Map *map, Ghost *ghost, Player *player);

/**
 * @brief Activate the ghost
 * @param ghost The ghost to activate
 */
void ghost_activate(Ghost *ghost);

/**
 * @brief Deactivate the ghost
 * @param ghost The ghost to deactivate
 */
void ghost_deactivate(Ghost *ghost);

/**
 * @brief Move the ghost to the spawn
 * @param ghost The ghost to move
 */
void ghost_move_to_spawn(Ghost *ghost);

# endif