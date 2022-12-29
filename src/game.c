#include <unistd.h>

#include "game.h"
#include "game_state.h"
#include "window.h"
#include "player.h"
#include "map.h"
#include "map_tile.h"

#define _XOPEN_SOURCE 500
#define FPS 60

#define PLAYER_START_X 5
#define PLAYER_START_Y 5


Game *game_create(int width, int height, int scale)
{
  Game *game = malloc(sizeof(*game));
  if (game == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire\n");
    return NULL;
  }

  // init game window for rendering
  game->window = window_create("Pacman", 640, 480);
  window_load_font(game->window, "../assets/fonts/OpenSans.ttf", 16);
  if (game->window == NULL) {
    return NULL;
  }

  // init map
  game->map = map_init(game->window, "../assets/maps/map1.txt", "../assets/textures/tiles5.png");
  if (game->map == NULL) {
    return NULL;
  }

  // init game score
  game->score = 0;

  // init game state
  game->state = STATE_MENU;
  // init player
  game->player = player_create(game->window, PLAYER_START_X * MAP_TILE_SIZE, PLAYER_START_Y * MAP_TILE_SIZE);

  // init ghost
  for (int i = 0; i < GHOST_AMOUNT; i++)
  {
    game->ghosts[i] = ghost_create(game->window, 1 * MAP_TILE_SIZE, (i+1) * MAP_TILE_SIZE, i+1);
  }
  
  return game;
}

void game_destroy(Game *game)
{
  if (game == NULL) {
    return;
  }

  window_destroy(game->window);
  player_destroy(game->player);
  for (int i = 0; i < GHOST_AMOUNT; i++)
  {
    ghost_destroy(game->ghosts[i]);
  }
  map_destroy(game->map);
  free(game);
}

void game_run(Game *game)
{
  if (game == NULL) {
    return;
  }

  while (1)
  {
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        printf("quit\n");
        break;
      }
    }

    window_clear(game->window);
    
    map_render(game->map, game->window);

    game_check_collision(game);
    player_update(game->player, game->window);

    for (int i = 0; i < GHOST_AMOUNT; i++)
    {
      ghost_update(game->map, game->ghosts[i], game->player);
    }

    switch ((int)game->state)
    {
      case STATE_MENU:
        game_state_menu_draw(game);
        break;
      case STATE_GAME:
        game_state_game_draw(game);
        break;
      case STATE_GAME_OVER:
        game_state_game_over_draw(game);
        break;
      default:
        break;
    }

    window_update(game->window);

    //SDL_Delay(10);
    usleep(1000 * 1000 / FPS);
  }
}

void game_check_collision(Game *game)
{

  Map *map = game->map;
  Player *player = game->player;
  Window *window = game->window;

  if (player->x < 0) {
    player->x = window->width - PLAYER_SIZE;
    return;
  }
  if (player->x > window->width - PLAYER_SIZE) {
    player->x = 0;
    return;
  }
  if (player->y < 0) {
    player->y = window->height - PLAYER_SIZE;
    return;
  }
  if (player->y > window->height - PLAYER_SIZE) {
    player->y = 0;
    return;
  }

  for (int i = 0; i < GHOST_AMOUNT; i++) {
    Ghost *ghost = game->ghosts[i];
    if (ghost->x < 0) {
      ghost->x = window->width - GHOST_SIZE;
      return;
    }
    if (ghost->x > window->width - GHOST_SIZE) {
      ghost->x = 0;
      return;
    }
    if (ghost->y < 0) {
      ghost->y = window->height - GHOST_SIZE;
      return;
    }
    if (ghost->y > window->height - GHOST_SIZE) {
      ghost->y = 0;
      return;
    }
  }

  int x = floor((player->x + PLAYER_SIZE/2) / MAP_TILE_SIZE);
  int y = floor((player->y + PLAYER_SIZE/2) / MAP_TILE_SIZE);

  for (int x = 0; x < map->cols; x++) {
    for (int y = 0; y < map->rows; y++) {
      if (map->map[x][y] != TILE_SPACE && map->map[x][y] != TILE_DOT && map->map[x][y] != TILE_POWER_UP) {
        float dx = player->x - x * MAP_TILE_SIZE;
        float dy = player->y - y * MAP_TILE_SIZE;

        if (sqrt(dx*dx + dy*dy) < PLAYER_SIZE) {
          if (player->direction == PLAYER_UP && dy > 0) {
            player->y = (y + 1) * MAP_TILE_SIZE;
          }
          if (player->direction == PLAYER_DOWN && dy < 0) {
            player->y = (y - 1) * MAP_TILE_SIZE;
          }
          if (player->direction == PLAYER_LEFT && dx > 0) {
            player->x = (x + 1) * MAP_TILE_SIZE;
          }
          if (player->direction == PLAYER_RIGHT && dx < 0) {
            player->x = (x - 1) * MAP_TILE_SIZE;
          }
        }
      }
    }
  }

  if (map->map[x][y] != TILE_SPACE && map->map[x][y] != TILE_DOT && map->map[x][y] != TILE_POWER_UP) {
    if (player->direction == PLAYER_UP) {
      player->y = (y + 1) * MAP_TILE_SIZE;
    }
    if (player->direction == PLAYER_DOWN) {
      player->y = (y - 1) * MAP_TILE_SIZE;
    }
    if (player->direction == PLAYER_LEFT) {
      player->x = (x + 1) * MAP_TILE_SIZE;
    }
    if (player->direction == PLAYER_RIGHT) {
      player->x = (x - 1) * MAP_TILE_SIZE;
    }
    player->moving = false;
  }

  if (map->map[x][y] == TILE_DOT) {
    map->map[x][y] = TILE_SPACE;
    game->score += 10;
    game->player->number_of_dots_eaten++;
  }

  if (map->map[x][y] == TILE_POWER_UP) {
    map->map[x][y] = TILE_SPACE;
    game->score += 50;
    game->player->invincible = true;
    game->player->invincible_timer = 0;
    game->player->number_of_ghosts_eaten = 0;
    game->player->number_of_power_pellets_eaten++;
  }

  for (int i = 0; i < GHOST_AMOUNT; i++) {
    if (ghost_check_collision(game->ghosts[i], player)) {
      if (player->invincible) {
        ghost_reset(game->ghosts[i]);
        player->number_of_ghosts_eaten++;
        game->score += 100 * player->number_of_ghosts_eaten;
      } else {
        player->lives--;
        printf("lives: %d\n", player->lives);
        if (player->lives == 0) {
          game->state = STATE_GAME_OVER;
        } else {
          player_reset(player);
          for (int i = 0; i < GHOST_AMOUNT; i++) {
            ghost_reset(game->ghosts[i]);
          }
        }
      }
    }
  }
}

void game_state_menu_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  display_score(game);
  display_lives(game);

  player_draw(game->player, game->window);

  for (int i = 0; i < GHOST_AMOUNT; i++)
  {
    ghost_render(game->ghosts[i], game->window);
    ghost_move(game->ghosts[i], game->player, game->map);
    if (game->player->invincible) {
      ghost_animation(game->ghosts[i], game->window);
    } else {
      char *path = malloc(sizeof(char) * 255);
      sprintf(path, "../assets/sprites/ghost_%d.png", i + 1);
      window_load_texture(game->window, path, &game->ghosts[i]->sprite);
    }
  }
}

void display_score(Game *game)
{
  char str[255];
  sprintf(str, "Score: %d", game->score);

  window_draw_text(game->window, 5, 2, str, 255, 255, 255);
}

void display_lives(Game *game)
{
  for (int i = game->player->lives; i > 0; i--) {
    SDL_Texture *hearth;
    window_load_texture(game->window, "../assets/sprites/hearth.png", &hearth);
    SDL_Rect rect = { 640 - (MAP_TILE_SIZE * i), 0, 32, 32 };
    window_draw_texture(game->window, hearth, NULL, &rect);
  }
}

void game_state_game_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  SDL_Rect rect = { 0, 0, 30, 30 };

  window_draw_rect(game->window, &rect, 0, 0, 255, 255);
}

void game_state_game_over_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  SDL_Rect rect = { 0, 0, 30, 30 };

  window_draw_rect(game->window, &rect, 0, 255, 0, 255);
}