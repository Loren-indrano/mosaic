// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef GAME_SDL_H
#define GAME_SDL_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Env_t Env;

/* **************************************************************** */
     
#ifdef __ANDROID__ 
#define PRINT(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__);  } while(0)
#define ERROR(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#else
#define PRINT(STR, ...) do { printf(STR, ##__VA_ARGS__); } while(0)
#define ERROR(STR, ...) do { fprintf(stderr, STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#endif

/* **************************************************************** */
     
#define APP_NAME "SDL2 Mosaic A14"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define DELAY 100

/* **************************************************************** */

Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]);
void render(SDL_Window* win, SDL_Renderer* ren, Env * env);
void clean(SDL_Window* win, SDL_Renderer* ren, Env * env);
bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);

/* **************************************************************** */

#endif
