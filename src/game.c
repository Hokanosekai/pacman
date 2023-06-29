#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

#include "game.h"
#include "game_state.h"
#include "window.h"
#include "player.h"
#include "map.h"
#include "map_tile.h"
#include "bonus.h"

#define _XOPEN_SOURCE 500

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

  // init game pseudo
  game->pseudo = malloc(sizeof(char) * PSEUDO_MAX_LENGTH);
  game->pseudo = "ANON";
  game->pseudo_index = 3;

  // init game state
  game->state = STATE_MENU;
  game->start_button_animation_frame = 0;
  game->is_paused = false;

  // init game fps
  game->display_fps = false;
  game->fps = 0;

  // init player
  game->player = player_create(game->window);
  if (game->player == NULL) return NULL;

  // init ghost
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    game->ghosts[i] = ghost_create(game->window, i+1);
    if (game->ghosts[i] == NULL) return NULL;
  }

  // init Bonus
  game->bonus = bonus_create(game->window, game->map);
  if (game->bonus == NULL) return NULL;

  // init best scores
  printf("Loading best scores...\n");
  game_load_best_scores(game);

  // init keys
  game->keys = SDL_GetKeyboardState(NULL);
  game->is_key_pressed = false;
  game->key_press_timer = 0;
  
  return game;
}

void game_destroy(Game *game)
{
  if (game == NULL) {
    return;
  }
  // destroy game window
  window_destroy(game->window);
  // destroy player
  player_destroy(game->player);
  // destroy ghosts
  for (int i = 0; i < GHOST_AMOUNT; i++)
  {
    ghost_destroy(game->ghosts[i]);
  }
  // destroy map
  map_destroy(game->map);
  // destroy bonus
  bonus_destroy(game->bonus);
  free(game);
}

void game_run(Game *game)
{
  if (game == NULL) return;

  bool render = false;

  float first_time = 0;
  float last_time = SDL_GetTicks() / 1000.0f;
  float passed_time = 0;
  float unprocessed_time = 0;

  float frame_time = 0;
  int frames = 0;

  while (game->state != STATE_EXIT)
  {
    render = true;

    first_time = SDL_GetTicks() / 1000.0f;
    passed_time = first_time - last_time;
    last_time = first_time;

    unprocessed_time += passed_time;
    frame_time += passed_time;

    while (unprocessed_time >= UPDATE_CAP)
    {
      unprocessed_time -= UPDATE_CAP;
      render = true;

      // update game
      game_update(game, (float) UPDATE_CAP);

      // game inputs
      game_input(game);

      if (frame_time >= 1.0f) {
        frame_time = 0;
        game->fps = frames;
        frames = 0;
      }
    }

    if (render) {
      // render game
      game_render(game);
      frames++;
    } else {
      SDL_Delay(1);
    }
  }
}

void game_update(Game *game, float delta)
{
  // Check for reset game
  if (game->last_key.keysym.mod & KMOD_LCTRL && game->keys[SDL_SCANCODE_R]) {
    game_reset(game);
    return;
  }

  // Check for exit game
  if (game->last_key.keysym.mod & KMOD_LALT && game->keys[SDL_SCANCODE_F4]) {
    game->state = STATE_EXIT;
    return;
  }

  // Check for display fps
  if (game->last_key.keysym.mod & KMOD_LCTRL && game->keys[SDL_SCANCODE_F]) {
    game->display_fps = !game->display_fps;
  }

  switch ((int)game->state)
  {
    case STATE_MENU:
      game_state_menu_update(game, delta);
      break;
    case STATE_GAME:
      game_state_game_update(game, delta);
      break;
    case STATE_GAME_OVER:
      game_state_game_over_update(game, delta);
      break;
    case STATE_EXIT:
      printf("Exit game\n");
      break;
    default:
      break;
  }
}

void game_input(Game *game)
{
  // get keyboard state
  game->keys = SDL_GetKeyboardState(NULL);

  // get last key pressed
  float start_time = SDL_GetTicks() / 1000.0f;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // check for exit game
    if (event.type == SDL_QUIT) game->state = STATE_EXIT;
    // check for key pressed
    if (event.type == SDL_KEYDOWN && !game->is_key_pressed) {
      game->is_key_pressed = true;
      game->last_key = event.key;
    }
    // check for key released
    if (event.type == SDL_KEYUP && start_time - game->key_press_timer > PRESS_KEY_DELAY) {
      game->is_key_pressed = false;
      game->key_press_timer = start_time;
    }
    // check for text input
    if (event.type == SDL_TEXTINPUT && game->state == STATE_GAME_OVER) {
      if (game->pseudo_index < PSEUDO_MAX_LENGTH) {
        char *tmp = malloc(sizeof(char) * (game->pseudo_index + 1));
        sprintf(tmp, "%s%s", game->pseudo[0] == ' '? "" : game->pseudo, event.text.text);
        game->pseudo = tmp;
        game->pseudo_index++;
      }
    }
  }
}

void game_render(Game *game)
{
  // clear window
  window_clear(game->window);

  // render map
  map_render(game->map, game->window);

  // render fps
  if (game->display_fps) display_fps(game);

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
    case STATE_EXIT:
      printf("Exit game\n");
      break;
    default:
      break;
  }

  // update window
  window_update(game->window);
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
    game->player->invincible_start_time = SDL_GetTicks() / 1000.0f;
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

  // check player collision with bonus
  if (bonus_check_collision(game->bonus, player)) {
    bonus_destroy(game->bonus);
    game->bonus = bonus_create(
      game->window,
      game->map
    );
    game->score += 1000;
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
  game->is_paused = false;

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

  // reset bonus
  bonus_destroy(game->bonus);
  game->bonus = bonus_create(
    game->window,
    game->map
  );

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

  // reset bonus
  bonus_destroy(game->bonus);
  game->bonus = bonus_create(
    game->window,
    game->map
  );
  
  // reset ghosts
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    ghost_reset(game->ghosts[i]);
    ghost_set_speed(game->ghosts[i], game->ghosts[i]->speed++);
  }
}

void display_fps(Game *game)
{
  char str[255];
  sprintf(str, "FPS: %d", game->fps);

  window_draw_text(
    game->window, 
    game->width - 5,
    game->height - 12,
    str,
    FPS_FONT_SIZE,
    WHITE_COLOR,
    ALIGN_RIGHT
  );
}

void display_start_button(Game *game)
{
  char str[255];
  sprintf(str, "Press Space to Start");

  window_draw_text(
    game->window, 
    game->width / 2,
    game->height / 2 - INSERT_COIN_FONT_SIZE/2, 
    str, 
    INSERT_COIN_FONT_SIZE, 
    WHITE_COLOR,
    ALIGN_CENTER
  );

  /*game->start_button_animation_frame++;
  if (game->start_button_animation_frame > START_BUTTON_ANIMATION_SPEED) {
    game->start_button_animation_frame = 0;
     window_draw_text(
      game->window, 
      game->width / 2,
      game->height / 2 - INSERT_COIN_FONT_SIZE/2, 
      str, 
      INSERT_COIN_FONT_SIZE, 
      WHITE_COLOR,
      ALIGN_CENTER
    );
  }*/
}

void display_best_scores(Game *game)
{
  for (int i = 0; i < 5; i++) {
    char *str = malloc(sizeof(char) * 255);
    sprintf(str, "%s", game->best_scores[i]);

    window_draw_text(
      game->window, 
      game->width / 2, 
      ((game->height / 4) * 3 - DEFAULT_FONT_SIZE) + (i * DEFAULT_FONT_SIZE), 
      str, 
      DEFAULT_FONT_SIZE, 
      WHITE_COLOR,
      ALIGN_CENTER
    );

    free(str);
  }
}

void game_state_menu_draw(Game *game)
{
  if (game == NULL) return;

  display_start_button(game);
  display_best_scores(game);
}

void game_state_menu_update(Game *game, float delta_time)
{
  if (game == NULL) return;

  if (game->keys[SDL_SCANCODE_SPACE]) {
    game->state = STATE_GAME;
  }
}

void display_score(Game *game)
{
  char str[255];
  sprintf(str, "Score: %d", game->score);

  window_draw_text(
    game->window, 
    5, 
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

void display_pause(Game *game)
{
  char str[255];
  sprintf(str, "PAUSE");

  window_draw_text(
    game->window, 
    game->width / 2, 
    game->height / 2, 
    str, 
    DEFAULT_FONT_SIZE, 
    WHITE_COLOR,
    ALIGN_CENTER
  );
}

void game_state_game_draw(Game *game)
{
  if (game == NULL) return;

  // display game info
  display_score(game);
  display_lives(game);
  display_level(game);
  if (game->is_paused) display_pause(game);

  // render bonus
  bonus_render(game->bonus, game->window, game->map);

  // render player
  player_render(game->player, game->window);

  // render ghosts
  for (int i = 0; i < GHOST_AMOUNT; i++) {
    ghost_render(game->ghosts[i], game->window);
  }
}

void game_state_game_update(Game *game, float delta_time)
{
  if (game == NULL) return;

  // check if player has pressed escape
  if (game->keys[SDL_SCANCODE_ESCAPE]) {
    game->is_paused = !game->is_paused;
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

  // update bonus
  if (!game->is_paused) bonus_update(game->bonus, game->map, game->player);

  // update player
  if (!game->is_paused) {
    player_update(game->map, game->player, game->keys);
  }
  // update ghosts
  if (!game->is_paused) {
    for (int i = 0; i < GHOST_AMOUNT; i++) {
      ghost_update(game->map, game->ghosts[i], game->player);
    }
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

void display_pseudo(Game *game, char *pseudo)
{
  window_draw_text(
    game->window, 
    game->width / 2, 
    game->height / 2 + DEFAULT_FONT_SIZE, 
    pseudo, 
    DEFAULT_FONT_SIZE, 
    WHITE_COLOR,
    ALIGN_CENTER
  );
}

void game_state_game_over_draw(Game *game)
{
  if (game == NULL) {
    return;
  }

  /*int min_score;
  char *min_pseudo = malloc(sizeof(char *) * 5);
  sscanf(game->best_scores[4], "%s %d", min_pseudo, &min_score);
  if (game->score < min_score) {
    game_reset(game);
    return;
  }*/

  display_game_over(game);
  display_insert_name(game);

  display_pseudo(game, game->pseudo);
}

void game_state_game_over_update(Game *game, float delta)
{
  if (game == NULL) {
    return;
  }

  char *temp = malloc(sizeof(char *) * 20);

  if (game->keys[SDL_SCANCODE_BACKSPACE]) {
    if (game->pseudo_index == 0) {
      game->pseudo = " ";
      return;
    }
    for (int i = 0; i < game->pseudo_index; i++) {
      temp[i] = game->pseudo[i];
    }
    game->pseudo = temp;
    game->pseudo_index--;
  }
  if (game->keys[SDL_SCANCODE_RETURN]) {
    game_insert_score(game, game->score, game->pseudo);
    game_save_best_scores(game);
    game_reset(game);
  }
}

void game_insert_score(Game *game, int score, char *pseudo)
{
  int index = 0, i = 0;
  // find the index where to insert the score
  while (index < 5) {
    char *temp = malloc(sizeof(char *) * 255);
    strcpy(temp, game->best_scores[i]);
    char *current_pseudo = strtok(temp, " : ");
    int current_score = atoi(strtok(NULL, " : "));
    if (current_score > score) {
      index++;
    } else {
      break;
    }
    i++;
  }

  // move all scores after the index
  for (int i = 4; i > index; i--) {
    strcpy(game->best_scores[i], game->best_scores[i - 1]);
  }

  // insert the score
  sprintf(game->best_scores[index], "%s : %d", pseudo, score);
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

  game->best_scores = malloc(sizeof(char *) * 5);

  while (fgets(str, 255, fp) != NULL) {
    game->best_scores[i] = malloc(sizeof(char) * 255);
    char *pseudo = strtok(str, " : ");
    int score = atoi(strtok(NULL, " : "));

    sprintf(game->best_scores[i], "%s : %d", pseudo, score);
    i++;
  }

  if (i < 5) {
    for (int j = i; j < 5; j++) {
      sprintf(game->best_scores[j], "XXX : 0");
    }
  }

  fclose(fp);
}