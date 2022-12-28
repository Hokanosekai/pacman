#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"
#include "window.h"
#include "game.h"
#include "game_state.h"
#include "map.h"
#include "map_tile.h"

#define PLAYER_SPEED 1
#define PLAYER_SIZE 32

Player *player_create(Window *window, int x, int y)
{
  Player *player = malloc(sizeof(Player));
  if (player == NULL) {
    return NULL;
  }

  player->x = x;
  player->y = y;
  player->speed = PLAYER_SPEED;
  player->direction = PLAYER_NULL;
  window_load_texture(window, "../assets/textures/pacman.png", &player->texture);

  return player;
}

void player_draw(Player *player, Window *window)
{
  SDL_Rect rect = {player->x, player->y, PLAYER_SIZE, PLAYER_SIZE};
  window_draw(window, player->texture, &rect);
}

void player_destroy(Player *player)
{
  if (player == NULL) {
    return;
  }

  SDL_DestroyTexture(player->texture);
  free(player);
}

void player_update(Player *player, Window *window)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_UP]) {
    player->direction = PLAYER_UP;
  }
  if (state[SDL_SCANCODE_DOWN]) {
    player->direction = PLAYER_DOWN;
  }
  if (state[SDL_SCANCODE_LEFT]) {
    player->direction = PLAYER_LEFT;
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    player->direction = PLAYER_RIGHT;
  }
  if (state[SDL_SCANCODE_SPACE]) {
    player->direction = PLAYER_NULL;
  }

  switch (player->direction) {
    case PLAYER_UP:
      player->y -= player->speed;
      break;
    case PLAYER_DOWN:
      player->y += player->speed;
      break;
    case PLAYER_LEFT:
      player->x -= player->speed;
      break;
    case PLAYER_RIGHT:
      player->x += player->speed;
      break;
    default:
      break;
  }
}

void player_check_collision(Player *player, Window *window, Map *map)
{
  if (player->x < 0) {
    player->x = window->width;
  }
  if (player->x > window->width - PLAYER_SIZE) {
    player->x = 0;
  }
  if (player->y < 0) {
    player->y = window->height;
  }
  if (player->y > window->height - PLAYER_SIZE) {
    player->y = 0;
  }

  int x = floor((player->x + PLAYER_SIZE/2) / MAP_TILE_SIZE);
  int y = floor((player->y + PLAYER_SIZE/2) / MAP_TILE_SIZE);

  //printf("x: %d, y: %d = %d\n", x, y, map->map[y][x]);

  if (map->map[x][y] != TILE_SPACE) {
    if (player->direction == PLAYER_UP) {
      player->y = (y + 1) * MAP_TILE_SIZE;
    }
    if (player->direction == PLAYER_DOWN) {
      player->y = (y - 1) * MAP_TILE_SIZE;
    }
    if (player->direction == PLAYER_LEFT) {
      player->x = (x + 1) * MAP_TILE_SIZE;
    }
    if (player->direction == PLAYER_RIGHT) {
      player->x = (x - 1) * MAP_TILE_SIZE;
    }
    player->direction = PLAYER_NULL;
  }
}