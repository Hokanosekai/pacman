#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"
#include "window.h"
#include "game.h"
#include "game_state.h"
#include "map.h"
#include "map_tile.h"

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
  player->animation_frame = 0;
  player->moving = false;
  player->dead = false;
  player->invincible = false;
  player->lives = PLAYER_LIVES;
  player->invincible_timer = 0;
  player->number_of_dots_eaten = 0;
  player->number_of_power_pellets_eaten = 0;
  player->number_of_ghosts_eaten = 0;
  window_load_texture(window, "../assets/sprites/pacman2.png", &player->sprite);

  return player;
}

void player_render(Player *player, Window *window)
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

void player_set_direction(Player *player, PlayerDirection direction)
{
  player->direction = direction;
}

void player_update(Map *map, Player *player)
{
  int x = player->x / MAP_TILE_SIZE;
  int y = player->y / MAP_TILE_SIZE;

  Tiles up = map_get_tile(map, x, y - 1);
  Tiles down = map_get_tile(map, x, y + 1);
  Tiles left = map_get_tile(map, x - 1, y);
  Tiles right = map_get_tile(map, x + 1, y);

  /*int up_distance = abs(player->y - (y * MAP_TILE_SIZE));
  int down_distance = abs(player->y - (y * MAP_TILE_SIZE));
  int left_distance = abs(player->x - (x * MAP_TILE_SIZE));
  int right_distance = abs(player->x - (x * MAP_TILE_SIZE));*/

  // up, down, left, right
  int dir_table[4] = { 0, 0, 0, 0 };

  if (tile_is_accessible(up)) dir_table[0] = 1;
  if (tile_is_accessible(down)) dir_table[1] = 1;
  if (tile_is_accessible(left)) dir_table[2] = 1;
  if (tile_is_accessible(right)) dir_table[3] = 1;

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_UP] && dir_table[0] == 1) {
    player_set_direction(player, PLAYER_UP);
    player->moving = true;
  }
  if (state[SDL_SCANCODE_DOWN] && dir_table[1] == 1) {
    player_set_direction(player, PLAYER_DOWN);
    player->moving = true;
  }
  if (state[SDL_SCANCODE_LEFT] && dir_table[2] == 1) {
    player_set_direction(player, PLAYER_LEFT);
    player->moving = true;
  }
  if (state[SDL_SCANCODE_RIGHT] && dir_table[3] == 1) {
    player_set_direction(player, PLAYER_RIGHT);
    player->moving = true;
  }
  if (state[SDL_SCANCODE_SPACE]) {
    player->moving = false;
  }

  // update player animation
  if (player->moving) {
    player->animation_timer++;
    if (player->animation_timer > PLAYER_ANIMATION_SPEED) {
      player->animation_timer = 0;
      player->animation_frame++;
      if (player->animation_frame > PLAYER_FRAMES - 1) {
        player->animation_frame = 0;
      }
    }
  }

  if (player->invincible) {
    player->invincible_timer++;
    if (player->invincible_timer > PLAYER_INVINCIBLE_TIME) {
      player->invincible = false;
      player->invincible_timer = 0;
    }
  }
}

void player_move(Player *player)
{
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
}

void player_move_to_spawn(Player *player)
{
  player->x = PLAYER_SPAWN_X * MAP_TILE_SIZE;
  player->y = PLAYER_SPAWN_Y * MAP_TILE_SIZE;
  player->direction = PLAYER_NULL;
}

void player_reset(Player *player)
{
  player->animation_frame = 0;
  player->moving = false;
  player->dead = false;
  player->invincible = false;
  player->invincible_timer = 0;
  player->number_of_dots_eaten = 0;
  player->number_of_power_pellets_eaten = 0;
  player->number_of_ghosts_eaten = 0;
  player_move_to_spawn(player);
}