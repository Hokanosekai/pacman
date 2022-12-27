# ifndef MAP_TILE_H
# define MAP_TILE_H

#define TILE_WALL_UP_COORDS {0, 0, 32, 32}
#define TILE_WALL_DOWN_COORDS {0, 32, 32, 32}
#define TILE_WALL_LEFT_COORDS {64, 0, 32, 32}
#define TILE_WALL_RIGHT_COORDS {96, 0, 32, 32}
#define TILE_SPACE_COORDS {32, 0, 32, 32}

typedef enum {
  TILE_WALL_UP = 4,
  TILE_WALL_DOWN = 3,
  TILE_WALL_LEFT = 2,
  TILE_WALL_RIGHT = 1,
  TILE_SPACE = 0,
} Tiles;

# endif