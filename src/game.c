#include "game.h"
#include "game_state.h"
#include "window.h"
#include "player.h"

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
  game->map = map_init(game->window, "../assets/maps/map1.txt", "../assets/textures/tiles3.png");
  if (game->map == NULL) {
    return NULL;
  }

  // init game score
  game->score = 0;

  // init game state
  game->state = STATE_MENU;
  if (game->state == NULL) {
    return NULL;
  }

  // init player
  game->player = player_create(game->window, PLAYER_START_X * MAP_TILE_SIZE, PLAYER_START_Y * MAP_TILE_SIZE);

  return game;
}

void game_destroy(Game *game)
{
  if (game == NULL) {
    return;
  }

  window_destroy(game->window);
  player_destroy(game->player);
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

    player_check_collision(game->player, game->window, game->map);
    player_update(game->player, game->window);

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

    SDL_Delay(10);
  }
}

void game_state_menu_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  SDL_Rect rect = { 0, 0, 30, 30 };
  SDL_Rect rect2 = { 50, 50, 100, 16 };

  //window_draw_rect(game->window, &rect, 255, 0, 0, 255);
  //window_draw_rect(game->window, &rect2, 255, 255, 0, 255);

  //window_draw_text(game->window, 50, 50, "Hello World", 255, 255, 255);
  player_draw(game->player, game->window);
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