# ifndef MAP_H
# define MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "window.h"
#include "map_tile.h"

#define MAP_TILE_SIZE 32

typedef struct {
  SDL_Texture *tile_map;
  FILE *map_file;
  Window *window;
  int **map;
  int cols, rows;
} Map;

/**
 * @brief Create a Map object
 * @param window Window
 * @param map_path Map path
 * @param tiles_textures_path Tiles textures path
 * @return Map*
 */
Map *map_init(Window *window, const char *map_path, const char *tiles_textures_path);

/**
 * @brief Render the Map object
 * @param map Map
 * @param window Window
 */
void map_render(Map *map, Window *window);

/**
 * @brief Destroy the Map object
 * @param map Map
 */
void map_destroy(Map *map);

/**
 * @brief Load the Map object
 * @param map Map
 */
void map_load(Map *map);

/**
 * @brief Get the Tile object
 * @param c Tile char
 * @return Tiles
 */
Tiles get_tile_from_char(char c);

/**
 * @brief Get the Tile object
 * @param map Map
 * @param x Tile x position
 * @param y Tile y position
 * @return int
 */
Tiles map_get_tile(Map *map, int x, int y);

/**
 * @brief Set the Tile object
 * @param map Map
 * @param x Tile x position
 * @param y Tile y position
 * @param tile Tile
 */
void map_set_tile(Map *map, int x, int y, Tiles tile);

/**
 * @brief Check collision between the player and the map
 * @param map Map
 * @param x Player x position
 * @param y Player y position
 * @return true
 * @return false
 */
bool map_check_collision(Map *map, int x, int y);

/**
 * @brief Count the number of dot in the map
 * @param map Map
 * @return int
 */
int map_count_dots(Map *map);

/**
 * @brief Count the number of power dot in the map
 * @param map Map
 * @return int
 */
int map_count_power_pellets(Map *map);

# endif