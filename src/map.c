#include <stdbool.h>

#include "map.h"
#include "window.h"
#include "map_tile.h"

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

  map_load(map);

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

void map_load(Map *map)
{
  char tile;
  int row = 0, col = 0;

  while (tile != EOF) {
    tile = fgetc(map->map_file);

    map->map[col][row] = get_tile_from_char(tile);

    if (tile != '\n' && tile != ' ') col++;
    if (col == map->cols) {
      col = 0;
      row++;
    }
  }
}

Tiles get_tile_from_char(char c)
{
  if (c == '1') return TILE_WALL_BOTTOM_LEFT_CORNER;
  if (c == '2') return TILE_WALL_BOTTOM_RIGHT_CORNER;
  if (c == '3') return TILE_WALL_TOP_LEFT_CORNER;
  if (c == '4') return TILE_WALL_TOP_RIGHT_CORNER;
  if (c == '0') return TILE_SPACE;
  if (c == '#') return TILE_FULL;
  if (c == 'd') return TILE_WALL_DOWN;
  if (c == 'u') return TILE_WALL_UP;
  if (c == 'l') return TILE_WALL_LEFT;
  if (c == 'r') return TILE_WALL_RIGHT;
  if (c == '=') return TILE_MIDDLE;
  if (c == '<') return TILE_MIDDLE_LEFT;
  if (c == '>') return TILE_MIDDLE_RIGHT;
  if (c == '.') return TILE_DOT;
  if (c == '(') return TILE_MIDDLE_CORNER_TOP_LEFT;
  if (c == ')') return TILE_MIDDLE_CORNER_TOP_RIGHT;
  if (c == '[') return TILE_MIDDLE_CORNER_BOTTOM_LEFT;
  if (c == ']') return TILE_MIDDLE_CORNER_BOTTOM_RIGHT;
  if (c == 'z') return TILE_MIDDLE_INTERSECTION_TOP;
  if (c == 's') return TILE_MIDDLE_INTERSECTION_BOTTOM;
  if (c == 'q') return TILE_MIDDLE_INTERSECTION_LEFT;
  if (c == 'f') return TILE_MIDDLE_INTERSECTION_RIGHT;
  if (c == '+') return TILE_MIDDLE_INTERSECTION;
  if (c == '|') return TILE_VERTICAL;
  if (c == '^') return TILE_VERTICAL_UP;
  if (c == 'v') return TILE_VERTICAL_DOWN;
  if (c == 'p') return TILE_POWER_UP;
  return TILE_SPACE;
}

void map_render(Map *map, Window *window)
{
  if (map == NULL) return;

  SDL_Rect src;
  SDL_Rect dst;
 
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
        case TILE_WALL_BOTTOM_LEFT_CORNER:
          src = (SDL_Rect) TILE_WALL_BOTTOM_LEFT_CORNER_COORDS;
          break;
        case TILE_WALL_BOTTOM_RIGHT_CORNER:
          src = (SDL_Rect) TILE_WALL_BOTTOM_RIGHT_CORNER_COORDS;
          break;
        case TILE_WALL_TOP_LEFT_CORNER:
          src = (SDL_Rect) TILE_WALL_TOP_LEFT_CORNER_COORDS;
          break;
        case TILE_WALL_TOP_RIGHT_CORNER:
          src = (SDL_Rect) TILE_WALL_TOP_RIGHT_CORNER_COORDS;
          break;
        case TILE_MIDDLE:
          src = (SDL_Rect) TILE_MIDDLE_COORDS;
          break;
        case TILE_MIDDLE_LEFT:
          src = (SDL_Rect) TILE_MIDDLE_LEFT_COORDS;
          break;
        case TILE_MIDDLE_RIGHT:
          src = (SDL_Rect) TILE_MIDDLE_RIGHT_COORDS;
          break;
        case TILE_FULL:
          src = (SDL_Rect) TILE_FULL_COORDS;
          break;
        case TILE_DOT:
          src = (SDL_Rect) TILE_DOT_COORDS;
          break;
        case TILE_MIDDLE_CORNER_TOP_LEFT:
          src = (SDL_Rect) TILE_MIDDLE_CORNER_TOP_LEFT_COORDS;
          break;
        case TILE_MIDDLE_CORNER_TOP_RIGHT:
          src = (SDL_Rect) TILE_MIDDLE_CORNER_TOP_RIGHT_COORDS;
          break;
        case TILE_MIDDLE_CORNER_BOTTOM_LEFT:
          src = (SDL_Rect) TILE_MIDDLE_CORNER_BOTTOM_LEFT_COORDS;
          break;
        case TILE_MIDDLE_CORNER_BOTTOM_RIGHT:
          src = (SDL_Rect) TILE_MIDDLE_CORNER_BOTTOM_RIGHT_COORDS;
          break;
        case TILE_MIDDLE_INTERSECTION_TOP:
          src = (SDL_Rect) TILE_MIDDLE_INTERSECTION_TOP_COORDS;
          break;
        case TILE_MIDDLE_INTERSECTION_BOTTOM:
          src = (SDL_Rect) TILE_MIDDLE_INTERSECTION_BOTTOM_COORDS;
          break;
        case TILE_MIDDLE_INTERSECTION_LEFT:
          src = (SDL_Rect) TILE_MIDDLE_INTERSECTION_LEFT_COORDS;
          break;
        case TILE_MIDDLE_INTERSECTION_RIGHT:
          src = (SDL_Rect) TILE_MIDDLE_INTERSECTION_RIGHT_COORDS;
          break;
        case TILE_MIDDLE_INTERSECTION:
          src = (SDL_Rect) TILE_MIDDLE_INTERSECTION_COORDS;
          break;
        case TILE_VERTICAL:
          src = (SDL_Rect) TILE_VERTICAL_COORDS;
          break;
        case TILE_VERTICAL_UP:
          src = (SDL_Rect) TILE_VERTICAL_UP_COORDS;
          break;
        case TILE_VERTICAL_DOWN:
          src = (SDL_Rect) TILE_VERTICAL_DOWN_COORDS;
          break;
        case TILE_POWER_UP:
          src = (SDL_Rect) TILE_POWER_UP_COORDS;
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

Tiles map_get_tile(Map *map, int x, int y)
{
  if (map == NULL) return TILE_SPACE;

  if (x < 0 || x >= map->cols || y < 0 || y >= map->rows) {
    return TILE_SPACE;
  }

  return map->map[x][y];
}

void map_set_tile(Map *map, int x, int y, Tiles tile)
{
  if (map == NULL) return;

  if (x < 0 || x >= map->cols || y < 0 || y >= map->rows) {
    return;
  }

  map->map[x][y] = tile;
}

bool map_check_collision(Map *map, int x, int y)
{
  if (map == NULL) return false;

  if (x < 0 || x % MAP_TILE_SIZE >= map->cols || y < 0 || y % MAP_TILE_SIZE >= map->rows) {
    return false;
  }

  Tiles tile = map->map[x % MAP_TILE_SIZE][y % MAP_TILE_SIZE];

  switch (tile) {
    case TILE_SPACE:
    case TILE_DOT:
    case TILE_POWER_UP:
      return false;
    default:
      return true;
  }
}

bool tile_is_accessible(Tiles tile)
{
  switch (tile) {
    case TILE_SPACE:
    case TILE_DOT:
    case TILE_POWER_UP:
      return true;
    default:
      return false;
  }
}

int map_count_dots(Map *map)
{
  int count = 0;

  for (int x = 0; x < map->cols; x++) {
    for (int y = 0; y < map->rows; y++) {
      if (map->map[x][y] == TILE_DOT) {
        count++;
        printf("count: %d\n", count);
      }
    }
  }

  return count;
}

int map_count_power_pellets(Map *map)
{
  int count = 0;

  for (int x = 0; x < map->cols; x++) {
    for (int y = 0; y < map->rows; y++) {
      if (map->map[x][y] == TILE_POWER_UP) {
        count++;
      }
    }
  }

  return count;
}