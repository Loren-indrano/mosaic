#include "game_aux.h"

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "queue.h"

void game_print_square(cgame g, uint i, uint j) {
  char *empty_char = "0123456789";

  if (game_get_color(g, i, j) == EMPTY) {
    if (game_get_constraint(g, i, j) == UNCONSTRAINED)
      printf("  ");
    else
      printf("%c ", empty_char[game_get_constraint(g, i, j)]);
  } else {
    if (game_get_color(g, i, j) == WHITE) {
      if (game_get_constraint(g, i, j) == UNCONSTRAINED) printf("□ ");
      // else printf("%c ",white_char[game_get_constraint(g,i,j)]);
      else {
        constraint cons = game_get_constraint(g, i, j);
        if (cons == 0) printf("🄋 ");
        if (cons == 1) printf("➀ ");
        if (cons == 2) printf("➁ ");
        if (cons == 3) printf("➂ ");
        if (cons == 4) printf("➃ ");
        if (cons == 5) printf("➄ ");
        if (cons == 6) printf("➅ ");
        if (cons == 7) printf("➆ ");
        if (cons == 8) printf("➇ ");
        if (cons == 9) printf("➈ ");
      }
    } else {
      if (game_get_color(g, i, j) == BLACK) {
        if (game_get_constraint(g, i, j) == UNCONSTRAINED) printf("■ ");
        // else printf("%c ",black_char[game_get_constraint(g,i,j)]);
        else {
          constraint cons = game_get_constraint(g, i, j);
          if (cons == 0) printf("🄌 ");
          if (cons == 1) printf("➊ ");
          if (cons == 2) printf("➋ ");
          if (cons == 3) printf("➌ ");
          if (cons == 4) printf("➍ ");
          if (cons == 5) printf("➎ ");
          if (cons == 6) printf("➏ ");
          if (cons == 7) printf("➐ ");
          if (cons == 8) printf("➑ ");
          if (cons == 9) printf("➒ ");
        }
      }
    }
  }
}

void print_line(int length) {
  printf("   ");
  for (uint i = 0; i < length; i++) {
    printf("-");
  }
  printf("\n");
}

void game_print(cgame g) {
  printf("\n   ");
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  for (uint i = 0; i < nb_cols; i++) {
    printf("%d ", i);
  }
  printf("\n");
  print_line(nb_cols * 2);
  for (uint i = 0; i < nb_rows; i++) {
    printf("%d |", i);
    for (uint j = 0; j < nb_cols; j++) {
      game_print_square(g, i, j);
    }
    printf("|\n");
  }
  print_line(nb_cols * 2);
}

game game_default(void) {
  constraint cons1[25] = {0,  -1, -1, 3,  -1, -1, 5,  -1, -1, -1, -1, -1, 4,
                          -1, 1,  6,  -1, 6,  3,  -1, -1, -1, -1, -1, -1};
  game g = game_new(cons1, NULL);
  return g;
}

game game_default_solution(void) {
  constraint cons1[25] = {0,  -1, -1, 3,  -1, -1, 5,  -1, -1, -1, -1, -1, 4,
                          -1, 1,  6,  -1, 6,  3,  -1, -1, -1, -1, -1, -1};
  color col1[25] = {WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
                    BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE,
                    WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK,
                    BLACK, BLACK, BLACK, WHITE};
  game g = game_new(cons1, col1);
  return g;
}
