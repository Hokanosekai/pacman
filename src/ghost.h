# ifndef GHOST_H
# define GHOST_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "map.h"

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

Ghost *ghost_create(Window *window, int x, int y, int ghost_number);

void ghost_destroy(Ghost *ghost);

void ghost_update(Ghost *ghost, Window *window);

void ghost_render(Ghost *ghost, Window *window);

void ghost_move(Ghost *ghost, Window *window, Map *map);

# endif