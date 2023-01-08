# ifndef _WINDOW_H_
# define _WINDOW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define FPS 30.0f
#define UPDATE_CAP 1.0f / FPS

#define RED_COLOR (SDL_Color) { 255, 0, 0, 255 }
#define GREEN_COLOR (SDL_Color) { 0, 255, 0, 255 }
#define BLUE_COLOR (SDL_Color) { 0, 0, 255, 255 }
#define WHITE_COLOR (SDL_Color) { 255, 255, 255, 255 }
#define BLACK_COLOR (SDL_Color) { 0, 0, 0, 255 }

typedef enum {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
} TextAlign;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *surface;
    TTF_Font *font;
    int width;
    int height;
    char *title;
} Window;

/**
 * @brief Cleanup SDL
 * @param window SDL_Window
 * @param renderer SDL_Renderer
 * @param texture SDL_Texture
 */
void cleanup(
    SDL_Window* window, 
    SDL_Renderer* renderer, 
    SDL_Texture* texture
);

/**
 * @brief Create a Window object
 * @param title Window title
 * @param width Window width
 * @param height Window height
 * @return Window*
 */
Window *window_create(char *title, int width, int height);

/**
 * @brief Destroy the Window object
 * @param window Window
 */
void window_destroy(Window *window);

/**
 * @brief Clear the window
 * @param window Window
 */
void window_clear(Window *window);

/**
 * @brief Update the window
 * @param window Window
 */
void window_update(Window *window);

/**
 * @brief Draw a texture on the window
 * @param window Window
 * @param texture SDL_Texture
 * @param rect SDL_Rect
 */
void window_draw(
    Window *window, 
    SDL_Texture *texture, 
    SDL_Rect *rect
);

/**
 * @brief Draw a rectangle on the window
 * @param window Window
 * @param rect SDL_Rect
 * @param color SDL_Color
 */
void window_draw_rect(
    Window *window, 
    SDL_Rect *rect, 
    SDL_Color color
);

/**
 * @brief Draw a line on the window
 * @param window Window
 * @param x1 X1
 * @param y1 Y1
 * @param x2 X2
 * @param y2 Y2
 * @param color SDL_Color
 */
void window_draw_line(
    Window *window, 
    int x1, int y1, 
    int x2, int y2, 
    SDL_Color color
);

/**
 * @brief Draw a circle on the window
 * @param window Window
 * @param x X
 * @param y Y
 * @param radius Radius
 * @param color SDL_Color
 */
void window_draw_circle(
    Window *window, 
    int x, int y, 
    int radius, 
    SDL_Color color
);

/**
 * @brief Draw a text on the window
 * @param window Window
 * @param x X
 * @param y Y
 * @param text Text
 * @param color SDL_Color
 * @param align TextAlign
 */
void window_draw_text(
    Window *window, 
    int x, int y, 
    const char *text, 
    int font_size, 
    SDL_Color color, 
    TextAlign align
);

/**
 * @brief Draw a texture on the window
 * @param window Window
 * @param texture SDL_Texture
 * @param src SDL_Rect
 * @param dst SDL_Rect
 */
void window_draw_texture(
    Window *window, 
    SDL_Texture *texture, 
    SDL_Rect *src, 
    SDL_Rect *dst
);

/**
 * @brief Load a texture from a path
 * @param window Window
 * @param path Path
 * @param texture SDL_Texture
 */
void window_load_texture(
    Window *window, 
    const char *path, 
    SDL_Texture **texture
);

/**
 * @brief Load a font from a path
 * @param window Window
 * @param path Path
 * @param size Font size
 */
void window_load_font(
    Window *window, 
    const char *path, 
    int size
);

/**
 * @brief Rotate texture 
 * @param window Window
 * @param texture Texture
 * @param angle Angle to rotate
 * @param flip Flip image verticaly
 */
void window_rotate_texture(
    Window *window, 
    SDL_Texture *texture, 
    SDL_Rect *rect, 
    double angle, 
    SDL_RendererFlip flip
);

/**
 * @brief Draw a sprite on the window
 * @param window Window
 * @param texture SDL_Texture
 * @param src SDL_Rect
 * @param dst SDL_Rect
 * @param angle Angle to rotate
 * @param flip Flip image verticaly
 */
void window_draw_sprite(
    Window *window, 
    SDL_Texture *texture, 
    SDL_Rect *src, 
    SDL_Rect *dst, 
    double angle, 
    SDL_RendererFlip flip
);

# endif