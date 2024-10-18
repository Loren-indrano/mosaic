/**
 * @file game.c
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ext.h"
#include "game_private.h"
#include "game_struct.h"
#include "queue.h"

/* ************************************************************************** */

int DIR2OFFSET[][2] = {
    [HERE] = {0, 0},      [UP] = {-1, 0},        [DOWN] = {1, 0},
    [LEFT] = {0, -1},     [RIGHT] = {0, 1},      [UP_LEFT] = {-1, -1},
    [UP_RIGHT] = {-1, 1}, [DOWN_LEFT] = {1, -1}, [DOWN_RIGHT] = {1, 1}};
direction FULL_DIR[] = {UP,       DOWN,      LEFT,       RIGHT, UP_LEFT,
                        UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, HERE};
direction ORTHO_DIR[] = {UP, DOWN, LEFT, RIGHT, HERE};
uint DIR_SIZES[] = {
    [FULL] = 9, [ORTHO] = 5, [FULL_EXCLUDE] = 8, [ORTHO_EXCLUDE] = 4};
direction* DIR_ARRAYS[] = {[FULL] = FULL_DIR,
                           [ORTHO] = ORTHO_DIR,
                           [FULL_EXCLUDE] = FULL_DIR,
                           [ORTHO_EXCLUDE] = ORTHO_DIR};

/* ************************************************************************** */
/*                                 GAME BASIC                                 */
/* ************************************************************************** */

game game_new(constraint* constraints, color* colors)
{
  return game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, constraints, colors, false,
                      FULL);
}

/* ************************************************************************** */

game game_new_empty(void)
{
  return game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, FULL);
}

/* ************************************************************************** */

game game_copy(cgame g)
{
  game gg = game_new_empty_ext(g->nb_rows, g->nb_cols, g->wrapping, g->neigh);
  memcpy(gg->squares, g->squares, g->nb_rows * g->nb_cols * sizeof(square));
  return gg;
}

/* ************************************************************************** */

bool game_equal(cgame g1, cgame g2)
{
  assert(g1 && g2);

  if (g1->nb_rows != g2->nb_rows) return false;
  if (g1->nb_cols != g2->nb_cols) return false;

  for (uint i = 0; i < g1->nb_rows; i++)
    for (uint j = 0; j < g1->nb_cols; j++) {
      color c1 = COLOR(g1, i, j);
      color c2 = COLOR(g2, i, j);
      constraint n1 = CONSTRAINT(g1, i, j);
      constraint n2 = CONSTRAINT(g2, i, j);
      if (n1 != n2 || c1 != c2) return false;
    }

  if (g1->wrapping != g2->wrapping) return false;
  if (g1->neigh != g2->neigh) return false;

  return true;
}

/* ************************************************************************** */

void game_delete(game g)
{
  if (!g) return;
  free(g->squares);
  queue_free_full(g->undo_stack, free);
  queue_free_full(g->redo_stack, free);
  free(g);
}

/* ************************************************************************** */

void game_set_constraint(game g, uint i, uint j, constraint n)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  assert(n >= MIN_CONSTRAINT && n <= MAX_CONSTRAINT);
  CONSTRAINT(g, i, j) = n;
}

/* ************************************************************************** */

void game_set_color(game g, uint i, uint j, color c)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  assert(c == BLACK || c == WHITE || c == EMPTY);
  COLOR(g, i, j) = c;
}

/* ************************************************************************** */

constraint game_get_constraint(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  return CONSTRAINT(g, i, j);
}

/* ************************************************************************** */

color game_get_color(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  return COLOR(g, i, j);
}

/* ************************************************************************** */

status game_get_status(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);

  int nb_expected_black = CONSTRAINT(g, i, j);
  int nb_black = game_nb_neighbors(g, i, j, BLACK);
  int nb_white = game_nb_neighbors(g, i, j, WHITE);
  int nb_empty = game_nb_neighbors(g, i, j, EMPTY);

  // === unconstrained square === //

  if (nb_expected_black == UNCONSTRAINED && nb_empty > 0) return UNSATISFIED;
  if (nb_expected_black == UNCONSTRAINED && nb_empty == 0) return SATISFIED;

  // === numbered square === //

  // errors
  if (nb_black > nb_expected_black)  // too many black squares
    return ERROR;
  if (nb_black + nb_empty < nb_expected_black)  // too many white squares
    return ERROR;

  // unsatisfied
  if (nb_empty > 0) return UNSATISFIED;

  // if constrained
  if (nb_black == nb_expected_black && nb_empty == 0) return SATISFIED;

  return ERROR;
}

/* ************************************************************************** */

bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint* pi_next,
                          uint* pj_next)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);

  // convert direction to offset
  int i_offset = DIR2OFFSET[dir][0];
  int j_offset = DIR2OFFSET[dir][1];

  // move to the next square in a given direction
  int ii = i + i_offset;
  int jj = j + j_offset;

  if (game_is_wrapping(g)) {
    ii = (ii + game_nb_rows(g)) % game_nb_rows(g);
    jj = (jj + game_nb_cols(g)) % game_nb_cols(g);
  }

  // check if next square at (ii,jj) is out of grid
  if (ii < 0 || ii >= (int)g->nb_rows) return false;
  if (jj < 0 || jj >= (int)g->nb_cols) return false;

  *pi_next = ii;
  *pj_next = jj;

  return true;
}

/* ************************************************************************** */

int game_nb_neighbors(cgame g, uint i, uint j, color c)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  uint count = 0;
  direction* dir_array = DIR_ARRAYS[g->neigh];
  uint dir_size = DIR_SIZES[g->neigh];

  for (uint n = 0; n < dir_size; n++) {
    direction dir = dir_array[n];
    uint ii, jj;
    bool valid = game_get_next_square(g, i, j, dir, &ii, &jj);
    if (valid && game_get_color(g, ii, jj) == c) count++;
  }

  return count;
}

/* ************************************************************************** */

void game_play_move(game g, uint i, uint j, color c)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  assert(c == BLACK || c == WHITE || c == EMPTY);

  color cc = COLOR(g, i, j);  // save current color
  COLOR(g, i, j) = c;         // set color

  // save history
  _stack_clear(g->redo_stack);
  move m = {i, j, cc, c};
  _stack_push_move(g->undo_stack, m);
}

/* ************************************************************************** */

bool game_won(cgame g)
{
  assert(g);
  assert(g);

  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++)
      if (game_get_status(g, i, j) != SATISFIED) return false;
  return true;
}

/* ************************************************************************** */

void game_restart(game g)
{
  assert(g);

  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++) game_set_color(g, i, j, EMPTY);

  // reset history
  _stack_clear(g->undo_stack);
  _stack_clear(g->redo_stack);
}

/* ************************************************************************** */
