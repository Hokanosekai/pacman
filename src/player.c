#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"
#include "window.h"
#include "game.h"
#include "game_state.h"
#include "map.h"
#include "map_tile.h"

#define PLAYER_SPEED 2
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
  window_load_texture(window, "../assets/sprites/pacman.png", &player->sprite);

  return player;
}

void player_draw(Player *player, Window *window)
{
  SDL_Rect rect = {player->x, player->y, PLAYER_SIZE, PLAYER_SIZE};
  SDL_Rect src = {PLAYER_SIZE * player->animation_frame, 0, PLAYER_SIZE, PLAYER_SIZE};

  if (player->direction == PLAYER_DOWN) {
    window_draw_sprite(window, player->sprite, &src, &rect, 90.0, SDL_FLIP_NONE);
  }
  if (player->direction == PLAYER_UP) {
    window_draw_sprite(window, player->sprite, &src, &rect, -90.0, SDL_FLIP_NONE);
  }
  if (player->direction == PLAYER_LEFT) {
    window_draw_sprite(window, player->sprite, &src, &rect, 0.0, SDL_FLIP_HORIZONTAL);
  }
  if (player->direction == PLAYER_RIGHT) {
    window_draw_sprite(window, player->sprite, &src, &rect, 0.0, SDL_FLIP_NONE);
  }
  if (player->direction == PLAYER_NULL) {
    window_draw_sprite(window, player->sprite, &src, &rect, 0.0, SDL_FLIP_NONE);
  }
  
}

void player_destroy(Player *player)
{
  if (player == NULL) {
    return;
  }

  SDL_DestroyTexture(player->sprite);
  free(player);
}

void player_update(Player *player, Window *window)
{
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_UP]) {
    player->direction = PLAYER_UP;
    player->moving = true;
  }
  if (state[SDL_SCANCODE_DOWN]) {
    player->direction = PLAYER_DOWN;
    player->moving = true;
  }
  if (state[SDL_SCANCODE_LEFT]) {
    player->direction = PLAYER_LEFT;
    player->moving = true;
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    player->direction = PLAYER_RIGHT;
    player->moving = true;
  }
  if (state[SDL_SCANCODE_SPACE]) {
    player->moving = false;
  }

  if (player->moving) {
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

  player->animation_frame++;
  if (player->animation_frame > 5) {
    player->animation_frame = 0;
  }
}

void player_check_collision(Player *player, Window *window, Map *map)
{
  //printf("player->x: %d\n", player->x);
  //printf("width: %d\n", window->width);
  if (player->x < 0) {
    //printf("player->x < 0\n");
    player->x = window->width - PLAYER_SIZE;
    return;
  }
  if (player->x > window->width - PLAYER_SIZE) {
    player->x = 0;
    return;
  }
  if (player->y < 0) {
    player->y = window->height;
    return;
  }
  if (player->y > window->height - PLAYER_SIZE) {
    player->y = 0;
    return;
  }

  int x = floor((player->x + PLAYER_SIZE/2) / MAP_TILE_SIZE);
  int y = floor((player->y + PLAYER_SIZE/2) / MAP_TILE_SIZE);

  for (int x = 0; x < map->cols; x++) {
    for (int y = 0; y < map->rows; y++) {
      if (map->map[x][y] != TILE_SPACE && map->map[x][y] != TILE_DOT) {
        float dx = player->x - x * MAP_TILE_SIZE;
        float dy = player->y - y * MAP_TILE_SIZE;

        if (dx*dx + dy*dy < MAP_TILE_SIZE*MAP_TILE_SIZE/2) {
          player->moving = false;
        }
      }
    }
  }

  if (map->map[x][y] != TILE_SPACE && map->map[x][y] != TILE_DOT) {
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
    player->moving = false;
    //player->direction = PLAYER_NULL;
  }

  if (map->map[x][y] == TILE_DOT) {
    map->map[x][y] = TILE_SPACE;
  }
}