# ifndef BONUS_H
# define BONUS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "map.h"
#include "window.h"
#include "player.h"

#define BONUS_TEXTURE_FILE "../assets/sprites/bonus.png"

#define BONUS_SPRITES_NUMBER 4

#define BONUS_RENDER_TIMER 400
#define BONUS_RENDER_BLINK_TIMER 350

#define BONUS_ANIMATION_TIMER 5

#define BONUS_SPRITE_SIZE 32

#define BONUS_MAX_INTERVAL 2000
#define BONUS_MIN_INTERVAL 1000

typedef struct {
  int x, y;
  int interval, timer;
  SDL_Texture *texture;
  int animation_timer;
  int render_timer;
  bool is_activate;
  SDL_Rect src;
} Bonus;

Bonus *bonus_create(Window *window, Map *map);

void bonus_destroy(Bonus *bonus);

void bonus_render(Bonus *bonus, Window *window, Map *map);

void bonus_activate(Bonus *bonus);

void bonus_deactivate(Bonus *bonus);

void bonus_update(Bonus *bonus, Map *map, Player *player);

void bonus_generate_position(Map *map, Bonus *bonus);

void bonus_generate_texture(Bonus *bonus);

void bonus_generate_interval(Bonus *bonus);

bool bonus_check_collision(Bonus *bonus, Player *player);

void bonus_reset(Bonus *bonus, Map *map);

# endif