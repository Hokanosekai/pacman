# ifndef BONUS_H
# define BONUS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "map.h"
#include "window.h"
#include "player.h"

#define BONUS_TEXTURE_FILE "../assets/sprites/bonus.png"

#define BONUS_SPRITES_NUMBER 8

#define BONUS_RENDER_TIME 20
#define BONUS_BLINK_TIME 15

#define BONUS_ANIMATION_COUNT 2
#define BONUS_ANIMATION_CAP UPDATE_CAP / BONUS_ANIMATION_COUNT

#define BONUS_SPRITE_SIZE 32

#define BONUS_MAX_INTERVAL 30
#define BONUS_MIN_INTERVAL 20

typedef struct {
  int x, y;
  float start_time;
  float interval, timer;
  SDL_Texture *texture;
  float animation_start_time;
  float render_start_time;
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