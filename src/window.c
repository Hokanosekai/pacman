#include "window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void cleanup(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture)
{
  if (texture != NULL) {
    SDL_DestroyTexture(texture);
  }
  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);
  }
  if (window != NULL) {
    SDL_DestroyWindow(window);
  }
  IMG_Quit();
  SDL_Quit();
}

Window *window_create(char *title, int width, int height)
{
  Window *window = malloc(sizeof(Window));
  window->width = width;
  window->height = height;
  window->title = title;
  window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
  if (window->window == NULL) {
      fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
      cleanup(NULL, NULL, NULL);
      return NULL;
  }

  window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
  if (window->renderer == NULL) {
      fprintf(stderr, "Erreur lors de la création du renderer : %s\n", SDL_GetError());
      cleanup(window->window, NULL, NULL);
      return NULL;
  }

  return window;
}

void window_destroy(Window *window)
{
  cleanup(window->window, window->renderer, NULL);
  free(window);
}

void window_clear(Window *window)
{
  SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
  SDL_RenderClear(window->renderer);
}

void window_update(Window *window)
{
  SDL_RenderPresent(window->renderer);
}

void window_draw(Window *window, SDL_Texture *texture, SDL_Rect *rect)
{
  SDL_RenderCopy(window->renderer, texture, NULL, rect);
}

void window_draw_rect(Window *window, SDL_Rect *rect, int r, int g, int b, int a)
{
  SDL_SetRenderDrawColor(window->renderer, r, g, b, a);
  SDL_RenderDrawRect(window->renderer, rect);
}

void window_draw_line(Window *window, int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
  SDL_SetRenderDrawColor(window->renderer, r, g, b, a);
  SDL_RenderDrawLine(window->renderer, x1, y1, x2, y2);
}

void window_draw_circle(Window *window, int x, int y, int radius, int r, int g, int b, int a)
{
  SDL_SetRenderDrawColor(window->renderer, r, g, b, a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        SDL_RenderDrawPoint(window->renderer, x + dx, y + dy);
      }
    }
  }
}

void window_load_texture(Window *window, const char *path, SDL_Texture **texture)
{
  SDL_Surface* surface = IMG_Load(path);
  if (surface == NULL) {
      fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", SDL_GetError());
      cleanup(window->window, window->renderer, NULL);
      return;
  }

  *texture = SDL_CreateTextureFromSurface(window->renderer, surface);
  SDL_FreeSurface(surface);
  if (*texture == NULL) {
      fprintf(stderr, "Erreur lors de la création de la texture : %s\n", SDL_GetError());
      cleanup(window->window, window->renderer, NULL);
      return;
  }
}