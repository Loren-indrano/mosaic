// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "game_sdl.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "game_random.h"

#define BACKGROUND "res/background-3.png"
#define HELP "res/help.png"
#define QUIT "res/quit.png"
#define RESET "res/reset.png"
#define REDO "res/redo.png"
#define UNDO "res/undo.png"
#define NAME "res/mosaicGame.png"
#define FONT "res/Regular.ttf"
#define FONTSIZE 10

int min(int a, int b) {
  if (a > b)
    return b;
  else
    return a;
}
/* **************************************************************** */

struct Env_t {
  SDL_Texture *name;
  SDL_Texture *background;
  SDL_Texture *help;
  SDL_Texture *reset;
  SDL_Texture *quit;
  SDL_Texture *redo;
  SDL_Texture *undo;
  TTF_Font *font;
  game g;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  if (argc > 1) {
    env->g = game_load(argv[1]);
  } else env->g = game_default();//env->g = game_random(5,5,false,FULL,true,50,50);

  if (!env->g) ERROR("Game not load\n");

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  env->name = IMG_LoadTexture(ren, NAME);
  if (!env->name) ERROR("IMG_LoadTexture: %s\n", NAME);

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  env->help = IMG_LoadTexture(ren, HELP);
  if (!env->help) ERROR("IMG_LoadTexture: %s\n", HELP);

  env->reset = IMG_LoadTexture(ren, RESET);
  if (!env->reset) ERROR("IMG_LoadTexture: %s\n", RESET);

  env->quit = IMG_LoadTexture(ren, QUIT);
  if (!env->quit) ERROR("IMG_LoadTexture: %s\n", QUIT);

  env->redo = IMG_LoadTexture(ren, REDO);
  if (!env->redo) ERROR("IMG_LoadTexture: %s\n", REDO);

  env->undo = IMG_LoadTexture(ren, UNDO);
  if (!env->undo) ERROR("IMG_LoadTexture: %s\n", UNDO);

  env->font = TTF_OpenFont(FONT, FONTSIZE);
  if (!env->font) ERROR("TTF_OpenFont: %s\n", FONT);

  TTF_CloseFont(env->font);
  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  if (win == NULL || ren == NULL || env == NULL) {
    fprintf(stderr, "Erreur : Paramètres invalides\n");
    return;
  }

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  if (env->background != NULL) {
    SDL_RenderCopy(ren, env->background, NULL, NULL);
  }

  SDL_Rect rect;
  rect.w = w/3.84;
  rect.h = h/2.16;

  SDL_QueryTexture(env->name, NULL, NULL, &rect.w, &rect.h);
  rect.w = w / 4.5;
  rect.h = rect.w;
  rect.x = w * 0.05;
  rect.y = h * 0.01;
  SDL_RenderCopy(ren, env->name, NULL, &rect);

  SDL_QueryTexture(env->help, NULL, NULL, &rect.w, &rect.h);
  rect.w = w / 4.5;
  rect.h = rect.w;
  rect.x = w * 0.05;
  rect.y = h * 0.154;
  SDL_RenderCopy(ren, env->help, NULL, &rect);
  

  SDL_QueryTexture(env->reset, NULL, NULL, &rect.w, &rect.h);
  rect.w = w / 4.5;
  rect.h = rect.w;
  rect.x = w * 0.05;
  rect.y = h * 0.275;
  SDL_RenderCopy(ren, env->reset, NULL, &rect);

  SDL_QueryTexture(env->quit, NULL, NULL, &rect.w, &rect.h);
  rect.w = w / 4.5;
  rect.h = rect.w;
  rect.x = w * 0.05;
  rect.y = h * 0.400;
  SDL_RenderCopy(ren, env->quit, NULL, &rect);

  SDL_QueryTexture(env->redo, NULL, NULL, &rect.w, &rect.h);
  rect.w = w / 4.5;
  rect.h = rect.w;
  rect.x = w * 0.4355;
  rect.y = h * 0.6;
  SDL_RenderCopy(ren, env->redo, NULL, &rect);

  SDL_QueryTexture(env->undo, NULL, NULL, &rect.w, &rect.h);
  rect.w = w / 4.5;
  rect.h = rect.w;
  rect.x = w * 0.275;
  rect.y = h * 0.6;
  SDL_RenderCopy(ren, env->undo, NULL, &rect);

  TTF_Font *Sans = TTF_OpenFont(FONT, 20);

  int nb_rows = game_nb_rows(env->g);
  int nb_cols = game_nb_cols(env->g);

  int cell_size = min(w / 2 / nb_rows, h / 2 / nb_cols);

  if (cell_size <= 0) {
    fprintf(stderr, "Erreur : Taille de jeu invalide\n");
    return;
  }

  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      int cell_x = j * cell_size + (w / 3);
      int cell_y = i * cell_size + (h / 5);

      SDL_Rect cell_rect = {cell_x, cell_y, cell_size, cell_size};
      color col = game_get_color(env->g, i, j);

      if (col == EMPTY) {
        SDL_SetRenderDrawColor(ren, 175, 68, 161, 255);
        SDL_RenderDrawRect(ren, &cell_rect);
      } else {
        if (col == BLACK) {
          SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        } else {
          SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        }
        SDL_RenderFillRect(ren, &cell_rect);
      }
      constraint c = game_get_constraint(env->g, i, j);
      if (c != UNCONSTRAINED) {
        char s[10];
        snprintf(s, 10, "%d", c);
        SDL_Color text_color = {190, 68, 161, 255};
        SDL_Color text_error_color = {255, 0, 0, 255};

        SDL_Surface *text_surface = TTF_RenderText_Solid(Sans, s, text_color);
        if (game_get_status(env->g,i,j)==ERROR) text_surface = TTF_RenderText_Solid(Sans, s, text_error_color);
        
        SDL_Texture *text_texture =
            SDL_CreateTextureFromSurface(ren, text_surface);

        SDL_Rect text_rect = {w / 3 + (j + 0.25) * cell_size,
                              h / 5 + (i + 0.30) * cell_size, cell_size / 2,
                              cell_size / 2};

        SDL_RenderCopy(ren, text_texture, NULL, &text_rect);

        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text_surface);
      }
      SDL_SetRenderDrawColor(ren, 175, 68, 161, 255);
      SDL_RenderDrawRect(ren, &cell_rect);
    }
  }

  if (game_won(env->g)) {
    TTF_Font* Sans = TTF_OpenFont(FONT, 24);

    char *s = "YOU WON";
    SDL_Color text_color = {190, 68, 161, 255}; 

    // Rendu du texte
    SDL_Surface* text_surface = TTF_RenderText_Solid(Sans, s, text_color);
    // Conversion de la surface en texture
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(ren, text_surface);

    SDL_Rect text_win_rect = {w/2,h/7, text_surface->w, text_surface->h};

    // Rendu de la texture du texte
    SDL_RenderCopy(ren, text_texture, NULL, &text_win_rect);
 
    // Libération de la surface et de la texture dutexte
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
  }

  SDL_RenderPresent(ren);
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }

/* Android events */
#ifdef __ANDROID__
  // else if (e->type == SDL_FINGERDOWN) {}

#else
  /* other events */

  else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    int nb_rows = game_nb_rows(env->g);
    int nb_cols = game_nb_cols(env->g);
    int cell_size = min(w / 2 / nb_rows, h / 2 / nb_cols);

    for (int i = 0; i < nb_rows; i++) {
      for (int j = 0; j < nb_cols; j++) {
        int cell_x = j * cell_size + (w / 3);
        int cell_y = i * cell_size + (h / 5);

        SDL_Rect cell_rect = {cell_x, cell_y, cell_size, cell_size};

        if (SDL_PointInRect(&mouse, &cell_rect)) {
          if (game_get_color(env->g, i, j) == EMPTY)
            game_play_move(env->g, i, j, WHITE);
          else if (game_get_color(env->g, i, j) == WHITE)
            game_play_move(env->g, i, j, BLACK);
          else
            game_play_move(env->g, i, j, EMPTY);
        }
      }
    }
    SDL_Rect rect;
    rect.w = w/3.84;
    rect.h = h/2.16;

    //cas click help
    SDL_Rect help_rect;
    help_rect.w = w / 9;
    help_rect.h = h / 13;
    help_rect.x =  (w * 0.05) + (rect.w/4);
    help_rect.y = h * 0.154 + (rect.w/2.5);
    if (SDL_PointInRect(&mouse, &help_rect)) {
      printf("Colorez en noir ou en blanc chaque case de la grille.\n");
      printf("Certaines cases contiennent un chiffre correspondant au nombre de cases noir se trouvant autour, \n");
      printf("la case elle-même peit faire partie du compte.\n");
    }
    //cas click reset
    SDL_Rect reset_rect;
    reset_rect.w = w / 9;
    reset_rect.h = h / 13;
    reset_rect.x =  (w * 0.05) + (rect.w/4);
    reset_rect.y = h * 0.275 + (rect.w/2.5);
    if (SDL_PointInRect(&mouse, &reset_rect)) {
      game_restart(env->g);
    }
    
    //cas click quit
    SDL_Rect quit_rect = {180, 570, 243, 91};
    quit_rect.w = w / 9;
    quit_rect.h = h / 13;
    quit_rect.x =  (w * 0.05) + (rect.w/4);
    quit_rect.y = h * 0.400 + (rect.w/2.5);
    if (SDL_PointInRect(&mouse, &quit_rect)) {
      if (game_won(env->g)) printf("YOU WON!\n");
      else printf("YOU LOST! Try again next time\n");
      //game_delete(env->g);
      return true;
    }

    //cas click redo
    SDL_Rect redo_rect;
    redo_rect.w = w / 9;
    redo_rect.h = h / 13;
    redo_rect.x =  (w * 0.4355) + (rect.w/4);
    redo_rect.y = h * 0.6 + (rect.w/2.5);
    if (SDL_PointInRect(&mouse, &redo_rect)) {
      game_redo(env->g);
    }

    //cas click undo
    SDL_Rect undo_rect;
    undo_rect.w = w / 9;
    undo_rect.h = h / 13;
    undo_rect.x =  (w * 0.275) + (rect.w/4);
    undo_rect.y = h * 0.6 + (rect.w/2.5);
    if (SDL_PointInRect(&mouse, &undo_rect)) {
      game_undo(env->g);
    }

  } else if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      //quitte le jeu
      case SDLK_ESCAPE:
        return true;
        break;
      //fichier sauvegarde de la partie
      case SDLK_s:
        game_save(env->g,"sdl_save.txt");
        break;
      //solution du jeu
      case SDLK_o:
        game_solve(env->g);
        break;
    }
  }

#endif
  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  if (env->name != NULL) SDL_DestroyTexture(env->name);
  if (env->background != NULL) SDL_DestroyTexture(env->background);
  if (env->help != NULL) SDL_DestroyTexture(env->help);
  if (env->reset != NULL) SDL_DestroyTexture(env->reset);
  if (env->quit != NULL) SDL_DestroyTexture(env->quit);
  if (env->redo != NULL) SDL_DestroyTexture(env->redo);
  if (env->undo != NULL) SDL_DestroyTexture(env->undo);

  free(env);
}

/* **************************************************************** */
