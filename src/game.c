#include <unistd.h>
#include <stdio.h>

#include "game.h"
#include "game_state.h"
#include "window.h"
#include "player.h"
#include "map.h"
#include "map_tile.h"
#include "bonus.h"

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
  if (game->window == NULL) return NULL;

  // loading font
  window_load_font(game->window, FONT_FILE, 16);

  // loading textures
  window_load_texture(
    game->window, 
    HEART_TEXTURE_FILE, 
    &game->heart_texture
  );

  // init map
  game->map = map_init(
    game->window, 
    LEVEL_FILE, 
    MAP_TEXTURE_FILE
  );
  if (game->map == NULL) return NULL;

  // init game score
  game->score = 0;
  game->level = 1;

  // init game state
  game->state = STATE_MENU;
  game->start_button_animation_frame = 0;

  // init player
  game->player = player_create(game->window);
  if (game->player == NULL) return NULL;

  // init ghost
  for (int i = 0; i < GHOST_AMOUNT; i++)
  {
    game->ghosts[i] = ghost_create(game->window, i+1);
    if (game->ghosts[i] == NULL) return NULL;
  }

  // init Bonus
  game->bonus = bonus_create(game->window, game->map);
  if (game->bonus == NULL) return NULL;

  printf("Loading best scores...\n");
  //game_load_best_scores(game);
  
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
      ghost->next_x = window->width - GHOST_SIZE;
      ghost->x = ghost->next_x;
      return;
    }
    if (ghost->x > window->width - GHOST_SIZE) {
      ghost->next_x = 0;
      ghost->x = ghost->next_x;
      return;
    }
    if (ghost->y < 0) {
      ghost->next_y = window->height - GHOST_SIZE;
      ghost->y = ghost->next_y;
      return;
    }
    if (ghost->y > window->height - GHOST_SIZE) {
      ghost->next_y = 0;
      ghost->y = ghost->next_y;
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
        player_kill(player);
        for (int i = 0; i < GHOST_AMOUNT; i++) {
          ghost_reset(game->ghosts[i]);
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

  // reset game
  game->score = 0;
  game->state = STATE_MENU;
  game->level = 1;

  // reset map
  map_destroy(game->map);
  game->map = map_init(
    game->window, 
    LEVEL_FILE, 
    MAP_TEXTURE_FILE
  );

  // reset player
  player_reset(game->player);
  player_reset_lives(game->player);

  // reset ghosts
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    ghost_reset(game->ghosts[i]);
    ghost_set_speed(game->ghosts[i], GHOST_SPEED);
  }
}

void game_next_level(Game *game)
{
  if (game == NULL) {
    return;
  }

  // reset map
  map_destroy(game->map);
  game->map = map_init(
    game->window, 
    LEVEL_FILE, 
    MAP_TEXTURE_FILE
  );

  // update game
  game->level++;
  game->score += 1000;
  game->state = STATE_GAME;

  // reset player
  player_reset(game->player);
  
  // reset ghosts
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    ghost_reset(game->ghosts[i]);
    ghost_set_speed(game->ghosts[i], game->ghosts[i]->speed++);
  }
}

void display_start_button(Game *game)
{
  char str[255];
  sprintf(str, "Insert Coin");

  game->start_button_animation_frame++;
  if (game->start_button_animation_frame > START_BUTTON_ANIMATION_SPEED) {
    game->start_button_animation_frame = 0;
     window_draw_text(
      game->window, 
      game->width / 2,
      game->height / 2 - INSERT_COIN_FONT_SIZE, 
      str, 
      INSERT_COIN_FONT_SIZE, 
      WHITE_COLOR,
      ALIGN_CENTER
    );
  }
}

void display_best_scores(Game *game)
{
  char str[255];
  int score;
  char name[100]; 
  for (int i = 0; i < 5; i++) {
    printf("%s", game->best_scores[i]);
    sscanf(game->best_scores[i], "%s %d", name, &score);
    sprintf(str, "%s. %d", name, score);
    window_draw_text(
      game->window, 
      game->width / 2, 
      ((game->height / 4) * 3 - DEFAULT_FONT_SIZE) + (i * DEFAULT_FONT_SIZE), 
      str, 
      DEFAULT_FONT_SIZE, 
      WHITE_COLOR,
      ALIGN_CENTER
    );
  }
}

void game_state_menu_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  display_start_button(game);
  //display_best_scores(game);

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_RETURN]) {
    game->state = STATE_GAME;
  }
}

void display_score(Game *game)
{
  char str[255];
  sprintf(str, "Score: %d", game->score);

  window_draw_text(
    game->window, 
    5 + MAP_TILE_SIZE, 
    12, 
    str, 
    DEFAULT_FONT_SIZE, 
    WHITE_COLOR,
    ALIGN_LEFT
  );
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

  window_draw_text(
    game->window, 
    game->width / 2, 
    12, 
    str, 
    DEFAULT_FONT_SIZE, 
    WHITE_COLOR,
    ALIGN_CENTER
  );
}

void game_state_game_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  // if player eats all dots go to next level
  if (game->player->number_of_dots_eaten == NUMBER_OF_DOT
    && game->player->number_of_power_pellets_eaten == NUMBER_OF_POWER_PELLET) {
    game_next_level(game);
    return;
  }

  // if player has no lives go to game over
  if (game->player->lives == 0) {
    game->state = STATE_GAME_OVER;
    return;
  }

  // check collision
  game_check_collision(game);

  // display game info
  display_score(game);
  display_lives(game);
  display_level(game);

  // update and render player
  player_update(game->map, game->player);
  player_render(game->player, game->window);

  // update and render ghosts
  for (int i = 0; i < GHOST_AMOUNT; i++)
  {
    ghost_update(game->map, game->ghosts[i], game->player);
    ghost_render(game->ghosts[i], game->window);
  }
}

void display_game_over(Game *game)
{
  char str[255];
  sprintf(str, "Game Over");

  window_draw_text(
    game->window, 
    game->width / 2, 
    game->height / 3 - GAME_OVER_FONT_SIZE, 
    str, 
    GAME_OVER_FONT_SIZE, 
    WHITE_COLOR,
    ALIGN_CENTER
  );
}

void display_insert_name(Game *game)
{
  char str[255];
  sprintf(str, "Insert Name");

  window_draw_text(
    game->window, 
    game->width / 2, 
    game->height / 2 - DEFAULT_FONT_SIZE, 
    str, 
    DEFAULT_FONT_SIZE, 
    WHITE_COLOR,
    ALIGN_CENTER
  );
}

void game_input(Game *game, SDL_Event *event, char *pseudo)
{
  if (game == NULL || event == NULL) return;


}

void game_state_game_over_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  int min_score;
  char min_pseudo[255];
  sscanf(game->best_scores[4], "%s %d", min_pseudo, &min_score);
  if (game->score < min_score) {
    game_reset(game);
    return;
  }

  display_game_over(game);
  display_insert_name(game);

  char pseudo[255];

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_RETURN]) {
    game_insert_score(game, game->score, pseudo);
    game_save_best_scores(game);
    game_reset(game);
  }

  if (state[SDL_SCANCODE_BACKSPACE]) {
    pseudo[strlen(pseudo) - 1] = '\0';
  }

  if (state[SDL_SCANCODE_A]) {
    strcat(pseudo, "A");
  }
}

void game_insert_score(Game *game, int score, char *pseudo)
{
  int index = 0;
  for (int i = 0; i < 5; i++) {
    int current_score;
    char pseudo[255];
    sscanf(game->best_scores[i], "%s %d", pseudo, &current_score);
    if (current_score > score) {
      index++;
    } else {
      break;
    }
  }

  for (int i = 4; i >= index; i--) {
    game->best_scores[i+1] = game->best_scores[i];
  }

  game->best_scores[index] = malloc(sizeof(char) * 255);
  sprintf(game->best_scores[index], "%s %d", pseudo, score);
}

void game_save_best_scores(Game *game)
{
  FILE *fp = fopen(SCORE_FILE, "w");
  if (fp == NULL) {
    return;
  }

  for (int i = 0; i < 5; i++) {
    fprintf(fp, "%s\n", game->best_scores[i]);
  }

  fclose(fp);
}

void game_load_best_scores(Game *game)
{
  FILE *fp = fopen(SCORE_FILE, "r");
  if (fp == NULL) {
    fp = fopen(SCORE_FILE, "w");
  }

  char str[255];
  int i = 0;

  while (fgets(str, 255, fp) != NULL) {
    game->best_scores[i] = malloc(sizeof(char) * 255);
    game->best_scores[i] = str;
    printf("%s", game->best_scores[i]);
    i++;
  }

  if (i < 5) {
    for (int j = i; j < 5; j++) {
      game->best_scores[j] = malloc(sizeof(char) * 255);
      sprintf(game->best_scores[j], "AAA 0");
    }
  }

  fclose(fp);
}