#include "map.h"
#include "window.h"
#include "map_tile.h"

#define MAP_TILE_SIZE 32

Map *map_init(Window *window, const char *map_path, const char *tiles_textures_path)
{
  Map *map = malloc(sizeof(*map));
  if (map == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire\n");
    return NULL;
  }

  map->window = window;
  window_load_texture(map->window, tiles_textures_path, &map->tile_map);
  if (map->tile_map == NULL) {
    return NULL;
  }

  map->map_file = fopen(map_path, "r");
  if (map->map_file == NULL) {
    fprintf(stderr, "Erreur d'ouverture du fichier %s\n", map_path);
    return NULL;
  }

  map->cols = window->width / MAP_TILE_SIZE;
  map->rows = window->height / MAP_TILE_SIZE;
  
  map->map = malloc(sizeof(int *) * map->cols);
  for (int i = 0; i < map->cols; i++) {
    map->map[i] = malloc(sizeof(int) * map->rows);
  }
  if (map->map == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire\n");
    return NULL;
  }

  return map;
}

void map_destroy(Map *map)
{
  if (map == NULL) {
    return;
  }

  fclose(map->map_file);
  free(map);
}

void map_render(Map *map, Window *window)
{
  if (map == NULL) {
    return;
  }

  char tile;
  SDL_Rect src;
  SDL_Rect dst;

  int row = 0, col = 0;

  while (tile != EOF) {
    tile = fgetc(map->map_file);
    
    if (tile == '0') {
      map->map[col][row] = TILE_SPACE;
    } else if (tile == 'd') {
      map->map[col][row] = TILE_WALL_DOWN;
    } else if (tile == 'l') {
      map->map[col][row] = TILE_WALL_LEFT;
    } else if (tile == 'r') {
      map->map[col][row] = TILE_WALL_RIGHT;
    } else if (tile == 'u') {
      map->map[col][row] = TILE_WALL_UP;
    }

    if (tile != '\n') col++;
    if (col == map->cols) {
      col = 0;
      row++;
    }
  }

  for (int y = 0; y < map->rows; y++) {
    for (int x = 0; x < map->cols; x++) {
      switch (map->map[x][y])
      {
        case TILE_SPACE:
          src = (SDL_Rect) TILE_SPACE_COORDS;
          break;
        case TILE_WALL_DOWN:
          src = (SDL_Rect) TILE_WALL_DOWN_COORDS;
          break;
        case TILE_WALL_LEFT:
          src = (SDL_Rect) TILE_WALL_LEFT_COORDS;
          break;
        case TILE_WALL_RIGHT:
          src = (SDL_Rect) TILE_WALL_RIGHT_COORDS;
          break;
        case TILE_WALL_UP:
          src = (SDL_Rect) TILE_WALL_UP_COORDS;
          break;
        default:
          src = (SDL_Rect) TILE_SPACE_COORDS;
          break;
      }
      dst = (SDL_Rect) { x * MAP_TILE_SIZE, y * MAP_TILE_SIZE, MAP_TILE_SIZE, MAP_TILE_SIZE };
      window_draw_texture(window, map->tile_map, &src, &dst);
    }
  }
}