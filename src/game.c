#include <unistd.h>
#include <stdio.h>

#include "game.h"
#include "game_state.h"
#include "window.h"
#include "player.h"
#include "map.h"
#include "map_tile.h"

#define _XOPEN_SOURCE 500
#define FPS 60

Game *game_create(int width, int height, int scale)
{
  Game *game = malloc(sizeof(*game));
  if (game == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire\n");
    return NULL;
  }

  game->width = width;
  game->height = height;

  // init game window for rendering
  game->window = window_create("Pacman", width, height);
  window_load_font(game->window, "../assets/fonts/OpenSans.ttf", 16);
  if (game->window == NULL) {
    return NULL;
  }

  // loading textures
  window_load_texture(
    game->window, 
    "../assets/sprites/heart.png", 
    &game->heart_texture
  );

  // init map
  game->map = map_init(
    game->window, 
    "../assets/maps/map1.txt", 
    "../assets/textures/tiles5.png"
  );
  if (game->map == NULL) {
    return NULL;
  }

  // init game score
  game->score = 0;
  game->level = 1;

  // init game state
  game->state = STATE_MENU;
  game->start_button_animation_frame = 0;

  // init player
  game->player = player_create(game->window);

  // init ghost
  for (int i = 0; i < GHOST_AMOUNT; i++)
  {
    game->ghosts[i] = ghost_create(game->window, i+1);
  }

  game->number_of_dot = map_count_dots(game->map);
  game->number_of_power_pellet = map_count_power_pellets(game->map);
  
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

    // clear window
    window_clear(game->window);
    // render map
    map_render(game->map, game->window);
    
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

    // update window
    window_update(game->window);

    SDL_Delay(10);
    usleep(1000 * 1000 / FPS);
  }
}

void game_check_collision(Game *game)
{

  Map *map = game->map;
  Player *player = game->player;
  Window *window = game->window;

  if (player->next_x < 0) {
    player->next_x = window->width - PLAYER_SIZE;
    player->x = player->next_x;;
    return;
  }
  if (player->next_x > window->width - PLAYER_SIZE) {
    player->next_x = 0;
    player->x = player->next_x;
    return;
  }
  if (player->next_y < 0) {
    player->next_y = window->height - PLAYER_SIZE;
    player->y = player->next_y;
    return;
  }
  if (player->next_y > window->height - PLAYER_SIZE) {
    player->next_y = 0;
    player->y = player->next_y;
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

  int x = (player->x + PLAYER_SIZE/2) / MAP_TILE_SIZE;
  int y = (player->y + PLAYER_SIZE/2) / MAP_TILE_SIZE;

  // check player collision with wall tile
  Tiles tile = map_get_tile(map, x, y);

  // check player collision with dot tile
  if (tile == TILE_DOT) {
    map->map[x][y] = TILE_SPACE;
    game->score += 10;
    game->player->number_of_dots_eaten++;
  }

  // check player collision with power up tile
  if (tile == TILE_POWER_UP) {
    map->map[x][y] = TILE_SPACE;
    game->score += 50;
    game->player->invincible = true;
    game->player->invincible_timer = 0;
    game->player->number_of_ghosts_eaten = 0;
    game->player->number_of_power_pellets_eaten++;
  }

  // check player collision with ghosts
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    if (ghost_check_collision(game->ghosts[i], player)) {
      if (player->invincible) {
        ghost_reset(game->ghosts[i]);
        player->number_of_ghosts_eaten++;
        game->score += 100 * player->number_of_ghosts_eaten;
      } else {
        player->lives--;
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

void game_reset(Game *game)
{
  if (game == NULL) {
    return;
  }

  game->score = 0;
  game->state = STATE_MENU;
  player_reset(game->player);
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    ghost_reset(game->ghosts[i]);
  }
}

void game_next_level(Game *game)
{
  if (game == NULL) {
    return;
  }

  map_destroy(game->map);
  game->map = map_init(
    game->window, 
    "../assets/maps/map1.txt", 
    "../assets/textures/tiles5.png"
  );

  game->level++;
  game->score += 1000;
  player_reset(game->player);
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    ghost_reset(game->ghosts[i]);
    ghost_set_speed(game->ghosts[i], game->ghosts[i]->speed++);
  }
}

void display_start_button(Game *game)
{
  char str[255];
  sprintf(str, "Insert Coin");

  int x = game->width / 2 - (strlen(str));
  int y = game->height / 2 - 50;

  window_draw_text(game->window, x, y, str, 255, 255, 255);
}

void display_best_scores(Game *game)
{
  FILE *fp = fopen("scores.txt", "rw");
  if (fp == NULL) {
    fp = fopen("scores.txt", "w");
  }

  char str[255];
  for (int i = 0; i < 5; i++) {
    if (fgets(str, 255, fp) == NULL) {
      sprintf(str, "%d: -----", i + 1);
    }
    window_draw_text(game->window, 5, 5 + i * 20, str, 255, 255, 255);
  }
}

void game_state_menu_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  game->start_button_animation_frame++;
  if (game->start_button_animation_frame > START_BUTTON_ANIMATION_SPEED) {
    game->start_button_animation_frame = 0;
    display_start_button(game);
  }

  display_best_scores(game);

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_RETURN]) {
    game->state = STATE_GAME;
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
    SDL_Rect rect = { game->width - (MAP_TILE_SIZE * i), 0, 32, 32 };
    window_draw_texture(game->window, game->heart_texture, NULL, &rect);
  }
}

void display_level(Game *game)
{
  char str[255];
  sprintf(str, "Level: %d", game->level);

  window_draw_text(game->window, (game->width / 2) - 5, 2, str, 255, 255, 255);
}

void game_state_game_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  // check collision
  game_check_collision(game);

  display_score(game);
  display_lives(game);
  display_level(game);

  player_update(game->map, game->player);
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    ghost_update(game->map, game->ghosts[i], game->player);
  }

  player_render(game->player, game->window);

  for (int i = 0; i < GHOST_AMOUNT; i++)
  {
    ghost_render(game->ghosts[i], game->window);
    if (game->player->invincible) {
      ghost_animation(game->ghosts[i], game->window);
    } else {
      char *path = malloc(sizeof(char) * 255);
      sprintf(path, "../assets/sprites/ghost_%d.png", i + 1);
      window_load_texture(game->window, path, &game->ghosts[i]->sprite);
    }
  }

  if (game->player->number_of_dots_eaten == NUMBER_OF_DOT
    && game->player->number_of_power_pellets_eaten == NUMBER_OF_POWER_PELLET) {
      game_next_level(game);
  }
}

void game_state_game_over_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  SDL_Rect rect = { 0, 0, 30, 30 };

  window_draw_rect(game->window, &rect, 0, 255, 0, 255);
}