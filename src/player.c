#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>

#include "player.h"
#include "window.h"
#include "game.h"
#include "game_state.h"
#include "map.h"
#include "map_tile.h"

Player *player_create(Window *window)
{
  Player *player = malloc(sizeof(Player));
  if (player == NULL) {
    return NULL;
  }

  player_move_to_spawn(player);
  player->speed = PLAYER_SPEED;
  player->speed_timer = 0;
  player->direction = PLAYER_NULL;
  player->next_direction = PLAYER_NULL;
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
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_UP]) player->next_direction = PLAYER_UP;
  if (state[SDL_SCANCODE_DOWN]) player->next_direction = PLAYER_DOWN;
  if (state[SDL_SCANCODE_LEFT]) player->next_direction = PLAYER_LEFT;
  if (state[SDL_SCANCODE_RIGHT]) player->next_direction = PLAYER_RIGHT;
  if (state[SDL_SCANCODE_SPACE]) player->next_direction = PLAYER_NULL;

  int next_x = player->next_x / MAP_TILE_SIZE;
  int next_y = player->next_y / MAP_TILE_SIZE;

  switch (player->next_direction)
  {
    case PLAYER_UP:
      next_y--;
      break;
    case PLAYER_DOWN:
      next_y++;
      break;
    case PLAYER_LEFT:
      next_x--;
      break;
    case PLAYER_RIGHT:
      next_x++;
      break;
    default:
      break;
  }

  Tiles next_tile = map_get_tile(map, next_x, next_y);
  
  /*int up_distance = abs(player->y - (y * MAP_TILE_SIZE));
  int down_distance = abs(player->y - (y * MAP_TILE_SIZE));
  int left_distance = abs(player->x - (x * MAP_TILE_SIZE));
  int right_distance = abs(player->x - (x * MAP_TILE_SIZE));*/

  if (tile_is_accessible(next_tile)) {
    player->direction = player->next_direction;
    player->next_x = next_x * MAP_TILE_SIZE;
    player->next_y = next_y * MAP_TILE_SIZE;
    player->moving = true;
  } else {
    player->next_x = player->x;
    player->next_y = player->y;
    player->moving = false;
  }

  // update player animation
  player->animation_timer += 1;
  if (player->animation_timer > PLAYER_ANIMATION_SPEED) {
    player->animation_timer = 0;
    if (player->moving) {
      player->animation_frame++;
      if (player->animation_frame > PLAYER_FRAMES - 1) {
        player->animation_frame = 0;
      }
    } else {
      player->animation_frame = 0;
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
  if (player->direction != PLAYER_NULL) {
    player->speed_timer++;
    if (player->speed_timer > PLAYER_SPEED) {
      player->speed_timer = 0;
      if (player->x < player->next_x) player->x++;
      if (player->x > player->next_x) player->x--;
      if (player->y < player->next_y) player->y++;
      if (player->y > player->next_y) player->y--;
    }
  }
}

void player_move_to_spawn(Player *player)
{
  player->x = PLAYER_SPAWN_X * MAP_TILE_SIZE;
  player->y = PLAYER_SPAWN_Y * MAP_TILE_SIZE;
  player->moving = false;
  player->next_x = player->x;
  player->next_y = player->y;
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