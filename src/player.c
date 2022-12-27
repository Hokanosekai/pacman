#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "player.h"
#include "window.h"
#include "game.h"
#include "game_state.h"

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
  window_load_texture(window, "assets/textures/pacman.png", &player->texture);

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

  player_check_collision(player, window);

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

void player_check_collision(Player *player, Window *window)
{
  if (player->x < 0) {
    player->x = 0;
  }
  if (player->x > window->width - PLAYER_SIZE) {
    player->x = window->width - PLAYER_SIZE;
  }
  if (player->y < 0) {
    player->y = 0;
  }
  if (player->y > window->height - PLAYER_SIZE) {
    player->y = window->height - PLAYER_SIZE;
  }
}