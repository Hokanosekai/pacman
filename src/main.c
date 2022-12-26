#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "window.h"


int main(int argc, char *argv[])
{
  Window *window;
  SDL_Surface* surface = NULL;
  SDL_Texture* texture = NULL;
  SDL_Rect rect = {40, 30, 100, 100};

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

  // Create window
  window = window_create("Pacman", 640, 480);
  if (window == NULL) {
    return EXIT_FAILURE;
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

    //window_load_texture(window, "assets/textures/2.jpg", &texture);
    //window_draw(window, texture, &rect);
    window_clear(window);

    window_draw_rect(window, &rect, 255, 0, 0, 255);

    window_draw_circle(window, 100, 100, 50, 255, 0, 0, 255);
    window_update(window);

    SDL_Delay(10);
    //printf("refresh\n");
  }

  return EXIT_SUCCESS;
}