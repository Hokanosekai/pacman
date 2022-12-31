#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <unistd.h>

#include "ghost.h"
#include "window.h"
#include "game.h"
#include "game_state.h"
#include "map.h"
#include "map_tile.h"
#include "player.h"

Ghost *ghost_create(Window *window, int ghost_number)
{
  Ghost *ghost = malloc(sizeof(Ghost));
  if (ghost == NULL) {
    return NULL;
  }

  ghost_move_to_spawn(ghost);
  ghost->speed = GHOST_SPEED;
  ghost->direction = GHOST_UP;
  ghost->next_direction = GHOST_UP;
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


  // Update ghost position
  int next_x = ghost->next_x / MAP_TILE_SIZE;
  int next_y = ghost->next_y / MAP_TILE_SIZE;

  Tiles next_up = map_get_tile(map, next_x, next_y - 1);
  Tiles next_down = map_get_tile(map, next_x, next_y + 1);
  Tiles next_left = map_get_tile(map, next_x - 1, next_y);
  Tiles next_right = map_get_tile(map, next_x + 1, next_y);

  int table[4] = {0, 0, 0, 0};

  if (tile_is_accessible(next_up)) table[0] = 1;
  if (tile_is_accessible(next_down)) table[1] = 1;
  if (tile_is_accessible(next_left)) table[2] = 1;
  if (tile_is_accessible(next_right)) table[3] = 1;

  table[ghost->direction] = 0;

  int sum = table[0] + table[1] + table[2] + table[3];

  for (int i = 0; i < 4; i++) {
    printf("%d ", table[i]);
  }
  printf("\n");

  // Update ghost direction
  if (!ghost->moving && sum > 1) {
    ghost->next_direction = ghost_get_direction(map, ghost, player);

    switch (ghost->next_direction)
    {
      case GHOST_UP:
        next_y--;
        break;
      case GHOST_DOWN:
        next_y++;
        break;
      case GHOST_LEFT:
        next_x--;
        break;
      case GHOST_RIGHT:
        next_x++;
        break;
      default:
        break;
    }

    Tiles next_tile = map_get_tile(map, next_x, next_y);

    if (tile_is_accessible(next_tile)) {
      ghost->direction = ghost->next_direction;
      ghost->moving = true;
      //printf("Ghost is moving to (%d, %d)\n", next_x, next_y);
      ghost->next_x = next_x * MAP_TILE_SIZE;
      ghost->next_y = next_y * MAP_TILE_SIZE;
    } else {
      ghost->next_x = ghost->x;
      ghost->next_y = ghost->y;
      ghost->moving = false;
    }
  } else if (sum == 1) {
    switch (ghost->next_direction)
    {
      case GHOST_UP:
        next_y--;
        break;
      case GHOST_DOWN:
        next_y++;
        break;
      case GHOST_LEFT:
        next_x--;
        break;
      case GHOST_RIGHT:
        next_x++;
        break;
      default:
        break;
    }

    if (table[0] == 0 && table[2] == 1 && ghost->direction == GHOST_UP) {
      ghost->next_direction = GHOST_LEFT;
      next_x--;
    } 
    if (table[0] == 0 && table[3] == 1 && ghost->direction == GHOST_UP) {
      ghost->next_direction = GHOST_RIGHT;
      next_x++;
    } 
    if (table[1] == 0 && table[2] == 1 && ghost->direction == GHOST_DOWN) {
      ghost->next_direction = GHOST_LEFT;
      next_x--;
    } 
    if (table[1] == 0 && table[3] == 1 && ghost->direction == GHOST_DOWN) {
      ghost->next_direction = GHOST_RIGHT;
      next_x++;
    } 
    if (table[2] == 0 && table[0] == 1 && ghost->direction == GHOST_LEFT) {
      ghost->next_direction = GHOST_UP;
      next_y--;
    } 
    if (table[2] == 0 && table[1] == 1 && ghost->direction == GHOST_LEFT) {
      ghost->next_direction = GHOST_DOWN;
      next_y++;
    } 
    if (table[3] == 0 && table[0] == 1 && ghost->direction == GHOST_RIGHT) {
      ghost->next_direction = GHOST_UP;
      next_y--;
    }
    if (table[3] == 0 && table[1] == 1 && ghost->direction == GHOST_RIGHT) {
      ghost->next_direction = GHOST_DOWN;
      next_y++;
    }

    Tiles next_tile = map_get_tile(map, next_x, next_y);

    if (tile_is_accessible(next_tile)) {
      ghost->direction = ghost->next_direction;
      ghost->moving = true;
      //printf("Ghost is moving to (%d, %d)\n", next_x, next_y);
      ghost->next_x = next_x * MAP_TILE_SIZE;
      ghost->next_y = next_y * MAP_TILE_SIZE;
    } else {
      ghost->next_x = ghost->x;
      ghost->next_y = ghost->y;
      ghost->moving = false;
    }
  }

  // Update ghost position
  ghost_move(ghost);  
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

void ghost_set_speed(Ghost *ghost, int speed)
{
  ghost->speed = speed;
}

void ghost_set_direction(Ghost *ghost, GhostDirection direction)
{
  ghost->direction = direction;
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
  ghost->direction = GHOST_UP;
  ghost->next_x = ghost->x;
  ghost->next_y = ghost->y;
}

bool ghost_check_collision(Ghost *ghost, Player *player)
{
  float dx = ghost->x - player->x;
  float dy = ghost->y - player->y;

  float distance = sqrt(dx * dx + dy * dy);

  return distance < MAP_TILE_SIZE/2;
}

void ghost_animation(Ghost *ghost, Window *window)
{
  window_load_texture(window, "../assets/sprites/ghost_inactive.png", &ghost->sprite);
}

void ghost_move(Ghost *ghost)
{
  if (ghost->x == ghost->next_x && ghost->y == ghost->next_y) {
    ghost->moving = false;
  } else {
    switch (ghost->direction)
    {
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
  }
}

GhostDirection ghost_get_direction(Map *map, Ghost *ghost, Player *player) 
{
  GhostDirection directions[3];
  int num_directions = 0;

  switch (ghost->direction)
  {
    case GHOST_UP:
      directions[num_directions++] = GHOST_LEFT;
      directions[num_directions++] = GHOST_RIGHT;
      directions[num_directions++] = GHOST_DOWN;
      break;
    case GHOST_DOWN:
      directions[num_directions++] = GHOST_LEFT;
      directions[num_directions++] = GHOST_UP;
      directions[num_directions++] = GHOST_RIGHT;
      break;
    case GHOST_LEFT:
      directions[num_directions++] = GHOST_DOWN;
      directions[num_directions++] = GHOST_UP;
      directions[num_directions++] = GHOST_RIGHT;
      break;
    case GHOST_RIGHT:
      directions[num_directions++] = GHOST_UP;
      directions[num_directions++] = GHOST_DOWN;
      directions[num_directions++] = GHOST_LEFT;
      break;
    default:
      break;
  }

  int rand_direction = rand() % num_directions;

  return directions[rand_direction];
}