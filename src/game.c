#include "game.h"
#include "game_state.h"
#include "window.h"


Game *game_create(int width, int height, int scale)
{
  Game *game = malloc(sizeof(*game));
  if (game == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire\n");
    return NULL;
  }

  game->window = window_create("Pacman", 640, 480);
  window_load_font(game->window, "assets/fonts/OpenSans.ttf", 16);
  if (game->window == NULL) {
    return NULL;
  }

  game->state = STATE_MENU;
  if (game->state == NULL) {
    return NULL;
  }

  return game;
}

void game_destroy(Game *game)
{
  if (game == NULL) {
    return;
  }

  window_destroy(game->window);
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

  window_draw_rect(game->window, &rect, 255, 0, 0, 255);
  window_draw_rect(game->window, &rect2, 255, 255, 0, 255);

  window_draw_text(game->window, "Hello World", 50, 50, 255, 255, 255);
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