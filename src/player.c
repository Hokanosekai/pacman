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
  player->animation_frame = 0;
  player->moving = false;
  player->dead = false;
  player->invincible = false;
  player->lives = PLAYER_LIVES;
  player->invincible_timer = 0;
  player->number_of_dots_eaten = 0;
  player->number_of_power_pellets_eaten = 0;
  player->number_of_ghosts_eaten = 0;
  window_load_texture(window, PLAYER_TEXTURE_FILE, &player->sprite);

  return player;
}

void player_render(Player *player, Window *window)
{
  SDL_Rect rect = {player->x, player->y, PLAYER_SIZE, PLAYER_SIZE};
  SDL_Rect src = {PLAYER_SIZE * player->animation_frame, 0, PLAYER_SIZE, PLAYER_SIZE};

  switch (player->direction)
  {
    case PLAYER_UP:
      window_draw_sprite(window, player->sprite, &src, &rect, -90.0, SDL_FLIP_NONE);
      break;
    case PLAYER_DOWN:
      window_draw_sprite(window, player->sprite, &src, &rect, 90.0, SDL_FLIP_NONE);
      break;
    case PLAYER_LEFT:
      window_draw_sprite(window, player->sprite, &src, &rect, 0.0, SDL_FLIP_HORIZONTAL);
      break;
    case PLAYER_RIGHT:
    case PLAYER_NULL:
      window_draw_sprite(window, player->sprite, &src, &rect, 0.0, SDL_FLIP_NONE);
      break;
  }
}

void player_destroy(Player *player)
{
  if (player == NULL) return;

  SDL_DestroyTexture(player->sprite);
  free(player);
}

void player_set_direction(Player *player, PlayerDirection direction)
{
  player->direction = direction;
}

void player_update(Map *map, Player *player, SDL_KeyboardEvent key)
{
  int next_x = player->next_x / MAP_TILE_SIZE;
  int next_y = player->next_y / MAP_TILE_SIZE;

  if (!player->moving) {

    // update player direction
    /*switch (key.keysym.sym)
    {
      case SDLK_UP:
        player->next_direction = PLAYER_UP;
        break;
      case SDLK_DOWN:
        player->next_direction = PLAYER_DOWN;
        break;
      case SDLK_LEFT:
        player->next_direction = PLAYER_LEFT;
        break;
      case SDLK_RIGHT:
        player->next_direction = PLAYER_RIGHT;
        break;
      case SDLK_SPACE:
        player->next_direction = PLAYER_NULL;
        break;
      default:
        break;
    }*/

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
  }

  Tiles next_tile = map_get_tile(map, next_x, next_y);

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

  // move player
  player_move(player);

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
  if (player->x == player->next_x && player->y == player->next_y) {
    player->moving = false;
  } else {
    switch (player->direction)
    {
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
  player->moving = false;
  player->next_x = player->x;
  player->next_y = player->y;
  player->direction = PLAYER_NULL;
  player->next_direction = PLAYER_NULL;
}

void player_kill(Player *player)
{
  player->dead = true;
  player->lives--;
  player_move_to_spawn(player);
  player->moving = false;
  player->invincible = false;
  player->invincible_timer = 0;
  player->animation_frame = 0;
  player->animation_timer = 0;
  player->speed_timer = 0;
  player->number_of_ghosts_eaten = 0;
}

void player_reset(Player *player)
{
  player->speed_timer = 0;
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

void player_reset_lives(Player *player)
{
  player->lives = PLAYER_LIVES;
}