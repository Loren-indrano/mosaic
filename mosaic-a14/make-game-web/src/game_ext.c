/**
 * @file game_ext.c
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#include "game_ext.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_private.h"
#include "game_struct.h"
#include "queue.h"

/* ************************************************************************** */
/*                                 GAME EXT                                   */
/* ************************************************************************** */

game game_new_ext(uint nb_rows, uint nb_cols, constraint* constraints,
                  color* colors, bool wrapping, neighbourhood neigh)
{
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  assert(g);
  color c = EMPTY;

  // set squares
  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++) {
      int n = constraints[i * nb_cols + j];
      if (colors != NULL) c = colors[i * nb_cols + j];
      CONSTRAINT(g, i, j) = n;
      COLOR(g, i, j) = c;
    }

  return g;
}

/* ************************************************************************** */

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        neighbourhood neigh)
{
  game g = (game)malloc(sizeof(struct game_s));
  assert(g);
  g->nb_rows = nb_rows;
  g->nb_cols = nb_cols;
  g->wrapping = wrapping;
  g->neigh = neigh;
  g->squares = (square*)calloc(g->nb_rows * g->nb_cols, sizeof(square));
  assert(g->squares);
  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++) {
      CONSTRAINT(g, i, j) = UNCONSTRAINED;
      COLOR(g, i, j) = EMPTY;
    }

  // initialize history
  g->undo_stack = queue_new();
  assert(g->undo_stack);
  g->redo_stack = queue_new();
  assert(g->redo_stack);

  return g;
}

/* ************************************************************************** */

uint game_nb_rows(cgame g) { return g->nb_rows; }

/* ************************************************************************** */

uint game_nb_cols(cgame g) { return g->nb_cols; }

/* ************************************************************************** */

bool game_is_wrapping(cgame g) { return g->wrapping; }

/* ************************************************************************** */

neighbourhood game_get_neighbourhood(cgame g) { return g->neigh; }

/* ************************************************************************** */

void game_undo(game g)
{
  assert(g);
  if (_stack_is_empty(g->undo_stack)) return;
  move m = _stack_pop_move(g->undo_stack);
  game_set_color(g, m.i, m.j, m.oldc);
  _stack_push_move(g->redo_stack, m);
}

/* ************************************************************************** */

void game_redo(game g)
{
  assert(g);
  if (_stack_is_empty(g->redo_stack)) return;
  move m = _stack_pop_move(g->redo_stack);
  game_set_color(g, m.i, m.j, m.newc);
  _stack_push_move(g->undo_stack, m);
}

/* ************************************************************************** */
