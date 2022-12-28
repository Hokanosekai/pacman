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
  player->lives = 3;
  player->invincible_timer = 0;
  player->number_of_dots_eaten = 0;
  player->number_of_power_pellets_eaten = 0;
  player->number_of_ghosts_eaten = 0;
  window_load_texture(window, "../assets/sprites/pacman2.png", &player->sprite);

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

  if (player->moving) {
    player->animation_frame = (player->animation_frame + 1) % 6;
  }

  if (player->invincible) {
    player->invincible_timer++;
    if (player->invincible_timer > 300) {
      player->invincible = false;
      player->invincible_timer = 0;
    }
  }
}

void player_reset(Player *player)
{
  player->x = 5 * MAP_TILE_SIZE;
  player->y = 5 * MAP_TILE_SIZE;
  player->direction = PLAYER_NULL;
  player->animation_frame = 0;
  player->moving = false;
  player->dead = false;
  player->invincible = false;
  player->invincible_timer = 0;
  player->number_of_dots_eaten = 0;
  player->number_of_power_pellets_eaten = 0;
  player->number_of_ghosts_eaten = 0;
}