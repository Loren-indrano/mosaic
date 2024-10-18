/**
 * @file game_tools.c
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#include "game_tools.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_struct.h"
#include "queue.h"

/* ************************************************************************** */
/*                               LOAD & SAVE                                  */
/* ************************************************************************** */


game game_load(char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "game_load : erreur fichier");
    exit(EXIT_FAILURE);
  }
  int nb_rows, nb_cols, wrapping, neigh;
  fscanf(f, "%d %d %d %d\n", &nb_rows, &nb_cols, &wrapping, &neigh);

  neighbourhood tab_dir[4] = {FULL, FULL_EXCLUDE, ORTHO, ORTHO_EXCLUDE};

  game g =
      game_new_empty_ext(nb_rows, nb_cols, (wrapping == 1), tab_dir[neigh]);

  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      char constr = fgetc(f);
      if (constr == '-') {
        game_set_constraint(g, i, j, UNCONSTRAINED);
      } else {
        game_set_constraint(g, i, j, constr - '0');
      }
      char color = fgetc(f);
      if (color == 'e') {
        game_set_color(g, i, j, EMPTY);
      } else if (color == 'b') {
        game_set_color(g, i, j, BLACK);
      } else if (color == 'w') {
        game_set_color(g, i, j, WHITE);
      }
    }
    fgetc(f);
  }
  fclose(f);
  return g;
}

/* ************************************************************************** */

char const_to_char(constraint cols) { return cols + '0'; }

void game_save(cgame g, char *filename) {
  if ((g == NULL) || (filename == NULL)) {
    fprintf(stderr, "game_save : erreur pointeur argument");
    exit(EXIT_FAILURE);
  }

  FILE *f = fopen(filename, "w");

  if (f == NULL) {
    fprintf(stderr, "game_save : erreur file");
    exit(EXIT_FAILURE);
  }

  int wrap;
  int neigh;

     wrap = 0;  // false

  neighbourhood tab_dir[4] = {FULL, FULL_EXCLUDE, ORTHO, ORTHO_EXCLUDE};
  for (int i = 0; i < 4; i++) {
    if (tab_dir[i] == game_get_neighbourhood(g)) neigh = i;
  }

  // première ligne du fichier : nb_rows nb cols wrapping neigh
  fprintf(f, "%d %d %d %d\n", game_nb_rows(g), game_nb_cols(g), wrap, neigh);

  char cons;
  char color;
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_constraint(g, i, j) == UNCONSTRAINED)
        cons = '-';
      else {
        cons = const_to_char(game_get_constraint(g, i, j));
      }

      if (game_get_color(g, i, j) == EMPTY)
        color = 'e';
      else if (game_get_color(g, i, j) == WHITE)
        color = 'w';
      else
        color = 'b';

      fprintf(f, "%c%c", cons, color);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}


/* ************************************************************************** */
/*                                  SOLVER                                    */
/* ************************************************************************** */

void game_solve_rec(game g, int rows, int cols) {
  if (game_has_error(g)){
    game_set_color(g,rows,cols,EMPTY);
    return;
  }

  if ((rows == 0) && (cols == 0)) {

    game_set_color(g, 0, 0, WHITE);
    if (game_won(g)){
      return;
    } 

    game_set_color(g, 0, 0, BLACK);
    if (game_won(g)) {
      return;
    } 

    game_set_color(g,rows,cols,EMPTY);
    return;
  }

  else {
    int new_rows = rows;
    int new_cols = cols - 1;

    if (cols == 0) {
      new_rows = rows - 1;
      new_cols = game_nb_cols(g) - 1;
    }

    game_set_color(g, rows, cols, WHITE);
    game_solve_rec(g, new_rows, new_cols);
    if (game_won(g)){
      game_set_color(g,rows,cols,WHITE);
      return;
    } 

    game_set_color(g, rows, cols, BLACK);
    game_solve_rec(g, new_rows, new_cols);
    if (game_won(g)){
      game_set_color(g,rows,cols,BLACK);
      return;
    } 

    game_set_color(g,rows,cols,EMPTY);
    return;
  }
}



bool game_solve(game g) {
  game_solve_rec(g, game_nb_rows(g) - 1, game_nb_cols(g) - 1);
  if (game_won(g)){
      return true;
  }
  game_restart(g);
  return false;
}

/* ************************************************************************** */

bool game_has_error(cgame g) {
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  for (uint i = 0; i < nb_rows; i++)
    for (uint j = 0; j < nb_cols; j++)
      if (game_get_status(g, i, j) == ERROR) return true;
  return false;
}

void game_nb_solutions_rec(game g, int rows, int cols, uint *count) {
  if (game_has_error(g)) return;

  if ((rows == 0) && (cols == 0)) {
    game g_copy = game_copy(g);

    game_set_color(g_copy, 0, 0, WHITE);
    if (game_won(g_copy)) (*count)++;

    game_set_color(g_copy, 0, 0, BLACK);
    if (game_won(g_copy)) (*count)++;

    game_delete(g_copy);
    return;
  }

  else {
    int new_rows = rows;
    int new_cols = cols - 1;

    if (cols == 0) {
      new_rows = rows - 1;
      new_cols = game_nb_cols(g) - 1;
    }

    game g_white = game_copy(g);
    game_set_color(g_white, rows, cols, WHITE);
    game_nb_solutions_rec(g_white, new_rows, new_cols, count);
    game_delete(g_white);

    game g_black = game_copy(g);
    game_set_color(g_black, rows, cols, BLACK);
    game_nb_solutions_rec(g_black, new_rows, new_cols, count);
    game_delete(g_black);
  }
}

uint game_nb_solutions(cgame g) {
  game g_copy = game_copy(g);
  uint count = 0;
  game_nb_solutions_rec(g_copy, game_nb_rows(g_copy) - 1,
                        game_nb_cols(g_copy) - 1, &count);
  game_delete(g_copy);
  return count;
}


/* ************************************************************************** */
/*                             Random Game Generator                          */
/* ************************************************************************** */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh,
                 bool with_solution, float black_rate, float constraint_rate)
{
  assert(black_rate >= 0.0f && black_rate <= 1.0f);
  assert(constraint_rate >= 0.0f && constraint_rate <= 1.0f);
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  assert(g);

  // fill the grid with random colors
  for (uint i = 0; i < nb_rows; i++)
  {
    for (uint j = 0; j < nb_cols; j++)
    {
      color c = (rand() < black_rate * (float)RAND_MAX) ? BLACK : WHITE;
      game_set_color(g, i, j, c);
    }
  }

  // fill the grid with actual constraint at random positions
  uint nb_squares = nb_rows * nb_cols;
  uint nb_constraints = constraint_rate * nb_squares;
  for (uint i = 0; i < nb_constraints; i++)
  {
    uint row = rand() % nb_rows;
    uint col = rand() % nb_cols;
    int nb_blacks = game_nb_neighbors(g, row, col, BLACK);
    game_set_constraint(g, row, col, nb_blacks);
  }

  // check solution
  if (!game_won(g))
  {
    game_delete(g);
    return NULL;
  }

  if (!with_solution)
    game_restart(g);
  return g;
}

/* ************************************************************************** */
