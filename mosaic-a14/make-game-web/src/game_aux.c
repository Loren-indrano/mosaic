/**
 * @file game_aux.c
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#include "game_aux.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_struct.h"

/* ************************************************************************** */

#define U UNCONSTRAINED

#define E EMPTY
#define B BLACK
#define W WHITE

/* ************************************************************************** */

/* default grid (numbers) */
static int grid_default_num[] = {
    0, U, U, 3, U, /* row 0 */
    U, 5, U, U, U, /* row 1 */
    U, U, 4, U, 1, /* row 2 */
    6, U, 6, 3, U, /* row 3 */
    U, U, U, U, U, /* row 4 */
};

/* ************************************************************************** */

/* default grid (color solution) */
static color grid_default_col[] = {
    W, W, B, W, W, /* row 0 */
    W, W, B, W, B, /* row 1 */
    B, B, B, W, W, /* row 2 */
    B, B, W, W, W, /* row 3 */
    B, B, B, B, W, /* row 4 */
};

/* ************************************************************************** */

game game_default(void) { return game_new(grid_default_num, NULL); }

/* ************************************************************************** */

game game_default_solution(void)
{
  return game_new(grid_default_num, grid_default_col);
}

/* ************************************************************************** */

void game_print(cgame g)
{
  assert(g);
  printf("     ");
  for (uint j = 0; j < game_nb_cols(g); j++) printf("%d ", j);
  printf("\n");
  printf("     ");
  for (uint j = 0; j < 2 * game_nb_cols(g); j++) printf("-");
  printf("\n");
  for (uint i = 0; i < game_nb_rows(g); i++) {
    printf("  %d |", i);
    for (uint j = 0; j < game_nb_cols(g); j++) {
      constraint n = game_get_constraint(g, i, j);
      color c = game_get_color(g, i, j);
      char* ch = _square2str(n, c);
      printf("%s ", ch);
    }
    printf("|\n");
  }
  printf("     ");
  for (uint j = 0; j < 2 * game_nb_cols(g); j++) printf("-");
  printf("\n");
}

/* ************************************************************************** */
