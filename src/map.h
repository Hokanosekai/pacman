# ifndef MAP_H
# define MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"

typedef struct {
  SDL_Texture *tile_map;
  FILE *map_file;
  Window *window;
  int **map;
  int cols, rows;
} Map;

Map *map_init(Window *window, const char *map_path, const char *tiles_textures_path);

void map_render(Map *map, Window *window);

void map_destroy(Map *map);

# endif