#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ghost.h"
#include "window.h"
#include "game.h"
#include "game_state.h"
#include "map.h"
#include "map_tile.h"

Ghost *ghost_create(Window *window, int x, int y, int ghost_number)
{
  Ghost *ghost = malloc(sizeof(Ghost));
  if (ghost == NULL) {
    return NULL;
  }

  ghost->x = x;
  ghost->y = y;
  ghost->speed = GHOST_SPEED;
  ghost->direction = GHOST_NULL;
  ghost->animation_frame = 0;
  ghost->is_active = false;
  ghost->moving = false;

  char *sprite_path = malloc(sizeof(char) * 100);
  sprintf(sprite_path, "../assets/sprites/ghost_%d.png", ghost_number);

  window_load_texture(window, sprite_path, &ghost->sprite);

  return ghost;
}

void ghost_destroy(Ghost *ghost)
{
  if (ghost == NULL) {
    return;
  }

  SDL_DestroyTexture(ghost->sprite);
  free(ghost);
}

void ghost_render(Ghost *ghost, Window *window)
{
  SDL_Rect rect = {ghost->x, ghost->y, GHOST_SIZE, GHOST_SIZE};
  SDL_Rect src = {GHOST_SIZE * ghost->animation_frame, 0, GHOST_SIZE, GHOST_SIZE};

  window_draw_sprite(window, ghost->sprite, &src, &rect, 0.0, SDL_FLIP_NONE);
}

void ghost_update(Ghost *ghost, Window *window)
{
  if (ghost->moving) {
    ghost->animation_frame = (ghost->animation_frame + 1) % 2;
  }
}