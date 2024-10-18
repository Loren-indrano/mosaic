#include "game_ext.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_struct.h"
#include "queue.h"

game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints,
                  color *colors, bool wrapping, neighbourhood neigh) {
  if (constraints == NULL) {
    fprintf(stderr, "game_new_ext : invalid parameters\n");
    exit(EXIT_FAILURE);
  }

  game g = safeMalloc(sizeof(game_S));
  g->rows = nb_rows;
  g->columns = nb_cols;
  g->grille = safeMalloc(g->rows * sizeof(Case *));
  for (int i = 0; i < g->rows; i++) {
    g->grille[i] = safeMalloc(g->columns * sizeof(Case));
  }
  g->history = queue_new();
  g->old_moves = queue_new();

  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      game_set_constraint(g, i, j, constraints[i * nb_rows + j]);
      if (colors == NULL)
        game_set_color(g, i, j, EMPTY);
      else {
        game_set_color(g, i, j, colors[i * nb_rows + j]);
      }
      // game_set_status(g, i, j);
    }
  }
  g->neigh = neigh;
  g->wrapping = wrapping;
  return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        neighbourhood neigh) {
  game g = safeMalloc(sizeof(game_S));
  g->rows = nb_rows;
  g->columns = nb_cols;
  g->grille = safeMalloc(g->rows * sizeof(Case *));
  for (int i = 0; i < g->rows; i++) {
    g->grille[i] = safeMalloc(g->columns * sizeof(Case));
  }
  g->history = queue_new();
  g->old_moves = queue_new();
  if (g != NULL) {
    for (uint i = 0; i < nb_rows; i++) {
      for (uint j = 0; j < nb_cols; j++) {
        game_set_color(g, i, j, EMPTY);
        game_set_constraint(g, i, j, -1);
      }
    }
  } else {
    fprintf(stderr, "game_new_empty : malloc fail\n");
    exit(EXIT_FAILURE);
  }
  g->neigh = neigh;
  g->wrapping = wrapping;
  return g;
}

// Loren
uint game_nb_rows(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "game_nb_rows : invalid game\n");
    exit(EXIT_FAILURE);
  }
  return g->rows;
}

// Loren
uint game_nb_cols(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "game_nb_cols : invalid game\n");
    exit(EXIT_FAILURE);
  }
  return g->columns;
}

// Loren
bool game_is_wrapping(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "game_is_wrapping : invalid game\n");
    exit(EXIT_FAILURE);
  }
  return g->wrapping;
}

// Loren
neighbourhood game_get_neighbourhood(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "game_get_neighbourhood : invalid game\n");
    exit(EXIT_FAILURE);
  }
  return g->neigh;
}

void game_undo(game g) {
  move *m = queue_pop_head(g->history);
  uint i = m->i;
  uint j = m->j;
  color c = m->old_col;
  game_set_color(g, i, j, c);
  queue_push_head(g->old_moves, m);
}

void game_redo(game g) {
  if (!queue_is_empty(g->old_moves)) {
    move *m = queue_pop_head(g->old_moves);
    queue_push_head(g->history, m);
    uint i = m->i;
    uint j = m->j;
    color c = m->new_col;
    game_set_color(g, i, j, c);
  }
}
