#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#include "ghost.h"
#include "window.h"
#include "game.h"
#include "game_state.h"
#include "map.h"
#include "map_tile.h"
#include "player.h"

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
  ghost->animation_timer = 0;
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

void ghost_update(Map *map, Ghost *ghost, Player *player)
{
  // Update ghost animation
  ghost->animation_timer++;
  if (ghost->animation_timer > GHOST_ANIMATION_SPEED) {
    ghost->animation_timer = 0;
    ghost->animation_frame++;
    if (ghost->animation_frame > GHOST_FRAMES - 1) {
      ghost->animation_frame = 0;
    }
  }

  // Update ghost direction
  ghost->direction = ghost_get_direction(map, ghost, player);
}

void ghost_reset(Ghost *ghost)
{
  ghost_move_to_spawn(ghost);
  ghost->speed = GHOST_SPEED;
  ghost->direction = GHOST_NULL;
  ghost->animation_frame = 0;
  ghost->is_active = false;
  ghost->moving = false;
}

void ghost_activate(Ghost *ghost)
{
  ghost->is_active = true;
}

void ghost_deactivate(Ghost *ghost)
{
  ghost->is_active = false;
}

void ghost_move_to_spawn(Ghost *ghost)
{
  ghost->x = GHOST_SPAWN_X * MAP_TILE_SIZE;
  ghost->y = GHOST_SPAWN_Y * MAP_TILE_SIZE;
}

bool ghost_check_collision(Ghost *ghost, Player *player)
{
  float dx = ghost->x - player->x;
  float dy = ghost->y - player->y;

  float distance = sqrt(dx * dx + dy * dy);

  return distance < 20;
}

void ghost_animation(Ghost *ghost, Window *window)
{
  window_load_texture(window, "../assets/sprites/ghost_inactive.png", &ghost->sprite);
}

void ghost_move(Ghost *ghost, Player *player, Map *map)
{
  switch (ghost->direction) {
    case GHOST_UP:
      ghost->y -= ghost->speed;
      break;
    case GHOST_DOWN:
      ghost->y += ghost->speed;
      break;
    case GHOST_LEFT:
      ghost->x -= ghost->speed;
      break;
    case GHOST_RIGHT:
      ghost->x += ghost->speed;
      break;
    default:
      break;
  }

  int x = ghost->x / MAP_TILE_SIZE;
  int y = ghost->y / MAP_TILE_SIZE;

  if (map_get_tile(map, x, y) != TILE_SPACE && map_get_tile(map, x, y) != TILE_DOT && map_get_tile(map, x, y) != TILE_POWER_UP) {
    if (ghost->direction == GHOST_UP) {
      ghost->y += (y + 1) * MAP_TILE_SIZE;
    } 
    if (ghost->direction == GHOST_DOWN) {
      ghost->y -= (y - 1) * MAP_TILE_SIZE;
    }
    if (ghost->direction == GHOST_LEFT) {
      ghost->x += (x + 1) * MAP_TILE_SIZE;
    } 
    if (ghost->direction == GHOST_RIGHT) {
      ghost->x -= (x - 1) * MAP_TILE_SIZE;
    }
    ghost->moving = false;
  }
}

GhostDirection ghost_get_direction(Map *map, Ghost *ghost, Player *player) 
{
  GhostDirection direction = ghost->direction;

  int x = ghost->x / MAP_TILE_SIZE;
  int y = ghost->y / MAP_TILE_SIZE;

  Tiles up = map_get_tile(map, x, y - 1);
  Tiles down = map_get_tile(map, x, y + 1);
  Tiles left = map_get_tile(map, x - 1, y);
  Tiles right = map_get_tile(map, x + 1, y);

  // up, down, left, right
  int dir_table[4] = {0, 0, 0, 0};

  if (tile_is_accessible(up)) dir_table[0] = 1;
  if (tile_is_accessible(down)) dir_table[1] = 1;
  if (tile_is_accessible(left)) dir_table[2] = 1;
  if (tile_is_accessible(right)) dir_table[3] = 1;

  /*printf("table: ");
  for (int i = 0; i < 4; i++) {
    printf("%d ", dir_table[i]);
  }
  printf("\n");*/

  if (rand() % 100 < 5) {
    int rand_dir = rand() % 4;

    while (dir_table[rand_dir] == 0) {
      rand_dir = rand() % 4;
    }

    if (rand_dir == direction) {
      return direction;
    }

    if (rand_dir == 0) {
      direction = GHOST_UP;
    } else if (rand_dir == 1) {
      direction = GHOST_DOWN;
    } else if (rand_dir == 2) {
      direction = GHOST_LEFT;
    } else if (rand_dir == 3) {
      direction = GHOST_RIGHT;  
    }
  }

  return direction;
}