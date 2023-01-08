#include <stdlib.h>

#include "bonus.h"
#include "map.h"
#include "window.h"
#include "player.h"

Bonus *bonus_create(Window *window, Map *map)
{
  Bonus *bonus = malloc(sizeof(Bonus));
  if (bonus == NULL) return NULL;

  // Load texture
  window_load_texture(window, BONUS_TEXTURE_FILE, &bonus->texture);

  bonus->is_activate = false;
  bonus->frame_count = 0;
  bonus->start_time = SDL_GetTicks() / 1000.0f;
  bonus->animation_start_time = 0;
  bonus->render_start_time = 0;

  // Generate x and y position
  bonus_generate_position(map, bonus);
  if (&bonus->x == NULL || &bonus->y == NULL) return NULL;

  // Generate sprite
  bonus_generate_texture(bonus);
  if (&bonus->src == NULL) return NULL;

  // Generate interval
  bonus_generate_interval(bonus);
  if (&bonus->interval == NULL) return NULL;

  return bonus;
}

void bonus_destroy(Bonus *bonus)
{
  if (bonus == NULL) return;

  SDL_DestroyTexture(bonus->texture);
  free(bonus); 
}

void bonus_render(Bonus *bonus, Window *window, Map *map)
{
  if (!bonus->is_activate) return;

  SDL_Rect dest = {bonus->x, bonus->y, BONUS_SPRITE_SIZE, BONUS_SPRITE_SIZE};
  float current_time = SDL_GetTicks() / 1000.0f;

  if (current_time - bonus->render_start_time >= BONUS_RENDER_TIME) {
    bonus_deactivate(bonus);
    bonus_reset(bonus, map);
    return;
  }

  if (current_time - bonus->render_start_time >= BONUS_BLINK_TIME) {
    if (current_time - bonus->animation_start_time >= BONUS_ANIMATION_CAP) {
      bonus->frame_count++;
      bonus->animation_start_time = current_time;
    }
    if (bonus->frame_count < BONUS_FRAME_CAP) {
      window_draw_texture(window, bonus->texture, &bonus->src, &dest);
    }
    if (bonus->frame_count >= BONUS_FRAME_MAX) {
      bonus->frame_count = 0;
    }
  } else {
    window_draw_texture(window, bonus->texture, &bonus->src, &dest);
  }
}

void bonus_update(Bonus *bonus, Map *map, Player *player)
{
  if (bonus->is_activate) return;

  float current_time = SDL_GetTicks() / 1000.0f;
  if (current_time - bonus->start_time >= bonus->interval) {
    bonus_activate(bonus);
    bonus->start_time = current_time;
  }
}

void bonus_activate(Bonus *bonus)
{
  bonus->is_activate = true;
  bonus->render_start_time = SDL_GetTicks() / 1000.0f;
}

void bonus_deactivate(Bonus *bonus)
{
  bonus->is_activate = false;
}

void bonus_generate_position(Map *map, Bonus *bonus)
{
  // Generate x and y position
  /*int x = rand() % MAP_TILE_SIZE;
  int y = rand() % MAP_TILE_SIZE;

  // Get tile
  Tiles tile = map_get_tile(map, x, y);

  // Check if tile is a dot
  while (tile != TILE_DOT)
  {
    x = rand() % MAP_TILE_SIZE;
    y = rand() % MAP_TILE_SIZE;
    tile = map_get_tile(map, x, y);
  }

  // Set bonus position
  bonus->x = x * BONUS_SPRITE_SIZE;
  bonus->y = y * BONUS_SPRITE_SIZE;

  // Set tile to empty
  map_set_tile(map, x, y, TILE_SPACE);*/
  bonus->x = 17 * BONUS_SPRITE_SIZE;
  bonus->y = 16 * BONUS_SPRITE_SIZE;
}

void bonus_generate_texture(Bonus *bonus)
{
  // Generate random sprite
  int x_offset = rand() % BONUS_SPRITES_NUMBER;
  // Set sprite
  bonus->src = (SDL_Rect) {
    x_offset * BONUS_SPRITE_SIZE,
    0, 
    BONUS_SPRITE_SIZE, 
    BONUS_SPRITE_SIZE
  };
}

void bonus_generate_interval(Bonus *bonus)
{
  bonus->interval = rand() % BONUS_MAX_INTERVAL + BONUS_MIN_INTERVAL;
}

bool bonus_check_collision(Bonus *bonus, Player *player)
{
  return player->x == bonus->x && player->y == bonus->y && bonus->is_activate;
}

void bonus_reset(Bonus *bonus, Map *map)
{
  bonus->is_activate = false;
  bonus->frame_count = 0;
  bonus->start_time = SDL_GetTicks() / 1000.0f;
  bonus->animation_start_time = 0;
  bonus->render_start_time = 0;

  // Regenerate position, sprite and interval
  bonus_generate_interval(bonus);
  bonus_generate_texture(bonus);
  bonus_generate_position(map, bonus);
}