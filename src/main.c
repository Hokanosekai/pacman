#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"
#include "game.h"
#include "game_state.h"


int main(int argc, char *argv[])
{
  Game *game;

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
    cleanup(NULL, NULL, NULL);
    return EXIT_FAILURE;
  }

  // Init SDL_image
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    fprintf(stderr, "Erreur d'initialisation de SDL_image : %s\n", IMG_GetError());
    cleanup(NULL, NULL, NULL);
    return EXIT_FAILURE;
  }

  // Create game instance
  game = game_create(640, 480, 1);
  if (game == NULL) {
    return EXIT_FAILURE;
  }

  // Run game
  game_run(game);

  // Destroy game instance
  game_destroy(game);
  
  return EXIT_SUCCESS;
}