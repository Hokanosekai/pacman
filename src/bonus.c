#include <stdlib.h>

#include "bonus.h"
#include "map.h"
#include "window.h"

Bonus *bonus_create(Window *window, Map *map)
{
  Bonus *bonus = malloc(sizeof(Bonus));
  if (bonus == NULL) return NULL;

  // Load texture
  window_load_texture(window, BONUS_TEXTURE_FILE, &bonus->texture);

  bonus->animation_timer = 0;
  bonus->render_timer = 0;
  bonus->is_activate = false;

  // Generate x and y position
  bonus_generate_position(map, bonus);
  if (&bonus->x == NULL || &bonus->y == NULL) return NULL;

  // Generate sprite
  bonus_generate_texture(bonus);
  if (&bonus->src == NULL) return NULL;

  return bonus;
}

void bonus_destroy(Bonus *bonus)
{
  if (bonus == NULL) return;

  SDL_DestroyTexture(bonus->texture);
  free(bonus); 
}

void bonus_render(Bonus *bonus, Window *window)
{
  if (!bonus->is_activate) return;

  SDL_Rect dest = {bonus->x, bonus->y, MAP_TILE_SIZE, MAP_TILE_SIZE};

  bonus->render_timer++;
  if (bonus->render_timer >= BONUS_RENDER_BLINK_TIMER) {
    bonus->animation_timer++;
    if (bonus->animation_timer > BONUS_ANIMATION_TIMER) {
      window_draw_texture(window, bonus->texture, &bonus->src, &dest);
      bonus->animation_timer = 0;
    }
  } else {
    window_draw_texture(window, bonus->texture, &bonus->src, &dest);
  }
  if (bonus->render_timer >= BONUS_RENDER_TIMER) {
    bonus_deactivate(bonus);
  }
}

void bonus_update(Bonus *bonus)
{
  
}

void bonus_activate(Bonus *bonus)
{
  bonus->is_activate = true;
}

void bonus_deactivate(Bonus *bonus)
{
  bonus->is_activate = false;
}

void bonus_generate_position(Map *map, Bonus *bonus)
{

}

void bonus_generate_texture(Bonus *bonus)
{
  int x_offset = rand() % BONUS_SPRITES_NUMBER - 1;
  bonus->src = (SDL_Rect) {
    x_offset * MAP_TILE_SIZE,
    0, 
    MAP_TILE_SIZE, 
    MAP_TILE_SIZE
  };
}