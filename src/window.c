#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "window.h"

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
  TTF_Quit();
}

Window *window_create(char *title, int width, int height)
{
  Window *window = malloc(sizeof(Window));
  window->width = width;
  window->height = height;
  window->title = title;
  window->font = NULL;

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
  TTF_CloseFont(window->font);
  cleanup(window->window, window->renderer, NULL);
  free(window);
}

void window_clear(Window *window)
{
  SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
  if (SDL_RenderClear(window->renderer) != 0) {
    fprintf(stderr, "Erreur lors du nettoyage de la fenêtre : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }
}

void window_update(Window *window)
{
  if (window->renderer == NULL) {
    fprintf(stderr, "Erreur lors de la mise à jour de la fenêtre : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }
  SDL_RenderPresent(window->renderer);
}

void window_draw(Window *window, SDL_Texture *texture, SDL_Rect *rect)
{
  if (SDL_RenderCopy(window->renderer, texture, NULL, rect) != 0) {
    fprintf(stderr, "Erreur lors du rendu de la texture : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, texture);
    return;
  }
}

void window_draw_rect(Window *window, SDL_Rect *rect, SDL_Color color)
{
  if (SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a) != 0) {
    fprintf(stderr, "Erreur lors du rendu du rectangle : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }
  if (SDL_RenderDrawRect(window->renderer, rect) != 0) {
    fprintf(stderr, "Erreur lors du rendu du rectangle : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }
}

void window_draw_line(
  Window *window, 
  int x1, int y1, 
  int x2, int y2, 
  SDL_Color color
) {
  if (SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a) != 0) {
    fprintf(stderr, "Erreur lors du rendu de la ligne : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }
  if (SDL_RenderDrawLine(window->renderer, x1, y1, x2, y2) != 0) {
    fprintf(stderr, "Erreur lors du rendu de la ligne : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }
}

void window_draw_circle(
  Window *window, 
  int x, int y, 
  int radius, 
  SDL_Color color
) {
  if (SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a) != 0) {
    fprintf(stderr, "Erreur lors du rendu du cercle : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx*dx + dy*dy) <= (radius * radius)) {
        if (SDL_RenderDrawPoint(window->renderer, x + dx, y + dy) != 0) {
          fprintf(stderr, "Erreur lors du rendu du cercle : %s\n", SDL_GetError());
          cleanup(window->window, window->renderer, NULL);
          return;
        }
      }
    }
  }
}

void window_draw_text(
  Window *window, 
  int x, int y, 
  const char *text, 
  int font_size, 
  SDL_Color color, 
  TextAlign align
) {
  SDL_Surface* surface = TTF_RenderText_Solid(window->font, text, color);
  if (surface == NULL) {
    fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(window->renderer, surface);
  if (texture == NULL) {
    fprintf(stderr, "[window_draw_text] Erreur lors de la création de la texture : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, NULL);
    return;
  }

  switch (align)
  {
    case ALIGN_LEFT:
      x = x;
      break;
    case ALIGN_CENTER:
      x = x - (strlen(text) * 8) / 2;
      break;
    case ALIGN_RIGHT:
      x = x - (strlen(text) * 8);
      break;
  }

  SDL_Rect rect = {x, y - (font_size/2), strlen(text) * 8, font_size};
  if (SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h) != 0) {
    fprintf(stderr, "Erreur lors de la récupération des dimensions de la texture : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, texture);
    return;
  }
  if (SDL_RenderCopy(window->renderer, texture, NULL, &rect) != 0) {
    fprintf(stderr, "[window_draw_text] Erreur lors du rendu de la texture : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, texture);
    return;
  }

  // free resources
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void window_draw_texture(
  Window *window, 
  SDL_Texture *texture, 
  SDL_Rect *src, 
  SDL_Rect *dst
) {
  if (SDL_RenderCopy(window->renderer, texture, src, dst) != 0) {
    fprintf(stderr, "[window_draw_texture] Erreur lors du rendu de la texture : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, texture);
    return;
  }
}

void window_load_texture(
  Window *window, 
  const char *path, 
  SDL_Texture **texture
) {
  SDL_Surface* surface = IMG_Load(path);
  if (surface == NULL) {
      fprintf(stderr, "[window_load_texture]\n Erreur lors du chargement de l'image : %s\n", SDL_GetError());
      cleanup(window->window, window->renderer, NULL);
      return;
  }

  *texture = SDL_CreateTextureFromSurface(window->renderer, surface);
  if (*texture == NULL) {
      fprintf(stderr, "[window_load_texture] Erreur lors de la création de la texture : %s\n", SDL_GetError());
      cleanup(window->window, window->renderer, NULL);
      return;
  }

  // free resources
  SDL_FreeSurface(surface);
}

void window_load_font(Window *window, const char *path, int size)
{
  window->font = TTF_OpenFont(path, size);
  if (window->font == NULL) {
      fprintf(stderr, "Erreur lors du chargement de la police : %s\n", TTF_GetError());
      cleanup(window->window, window->renderer, NULL);
      return;
  }
}

void window_rotate_texture(
  Window *window, 
  SDL_Texture *texture, 
  SDL_Rect *rect, 
  double angle, 
  SDL_RendererFlip flip
) {
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);
  SDL_Point center = { w/2, h/2 };
  if (SDL_RenderCopyEx(window->renderer, texture, NULL, rect, angle, &center, flip) != 0) {
    fprintf(stderr, "Erreur lors du rendu de la texture : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, texture);
    return;
  }
}


void window_draw_sprite(
  Window *window, 
  SDL_Texture *texture, 
  SDL_Rect *src, 
  SDL_Rect *dst, 
  double angle, 
  SDL_RendererFlip flip
) {
  SDL_Point center = { src->w/2, src->h/2 };
  if (SDL_RenderCopyEx(window->renderer, texture, src, dst, angle, &center, flip) != 0) {
    fprintf(stderr, "Erreur lors du rendu de la texture : %s\n", SDL_GetError());
    cleanup(window->window, window->renderer, texture);
    return;
  }
}