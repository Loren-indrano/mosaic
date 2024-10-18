#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_struct.h"

// Non inclus dans game.h
/**************************************************************************************/

bool valid_next_square(cgame g, uint i, uint j, direction dir) {
  if ((dir == UP) || (dir == UP_LEFT) || (dir == UP_RIGHT))
    if (i == 0) return false;
  if ((dir == DOWN) || (dir == DOWN_LEFT) || (dir == DOWN_RIGHT))
    if (i == g->rows - 1) return false;
  if ((dir == LEFT) || (dir == UP_LEFT) || (dir == DOWN_LEFT))
    if (j == 0) return false;
  if ((dir == RIGHT) || (dir == UP_RIGHT) || (dir == DOWN_RIGHT))
    if (j == g->columns - 1) return false;
  return true;
}

void *safeMalloc(int size) {
  void *tmp = malloc(size);
  if (tmp == NULL) {
    fprintf(stderr, "safeMalloc : malloc fail\n");
    exit(EXIT_FAILURE);
  }
  return tmp;
}

// Wendy
/*void game_set_status(game g, uint i, uint j) {
  int nb_empty = game_nb_neighbors(g,i,j,EMPTY);
  int nb_black = game_nb_neighbors(g, i, j, BLACK);
  int nb_white = game_nb_neighbors(g, i, j, WHITE);

  if (game_get_constraint(g, i, j) == UNCONSTRAINED) //nb BLACK == const ->
SATISFIED g->grille[i][j].stat = SATISFIED;

  else if ((nb_black> game_get_constraint(g, i, j))|| (nb_white >
((nb_white+nb_black+nb_empty)-game_get_constraint(g, i, j))))
    g->grille[i][j].stat = ERROR;
  else if(nb_black<game_get_constraint(g, i, j)) {
    g->grille[i][j].stat = UNSATISFIED;
  }
  else {
    g->grille[i][j].stat = SATISFIED;
  }
}*/

/**************************************************************************************/

game game_new(constraint *constraints, color *colors) {
  if (constraints == NULL) {
    fprintf(stderr, "game_new : invalid parameters\n");
    exit(EXIT_FAILURE);
  }

  // allocation game
  game g = safeMalloc(sizeof(game_S));
  g->rows = DEFAULT_SIZE;
  g->columns = DEFAULT_SIZE;
  g->grille = safeMalloc(g->rows * sizeof(Case *));
  for (int i = 0; i < g->rows; i++) {
    g->grille[i] = safeMalloc(g->columns * sizeof(Case));
  }
  g->history = queue_new();
  g->old_moves = queue_new();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_constraint(g, i, j, constraints[i * DEFAULT_SIZE + j]);
      if (colors != NULL) {
        game_set_color(g, i, j, colors[i * DEFAULT_SIZE + j]);
      } else {
        game_set_color(g, i, j, EMPTY);
      }
    }
  }
  g->wrapping = false;
  g->neigh = FULL;
  /*for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_status(g, i, j);
    }
  }*/
  return g;
}

// Loren
game game_new_empty(void) {
  // allocation game
  game g = safeMalloc(sizeof(game_S));
  g->rows = DEFAULT_SIZE;
  g->columns = DEFAULT_SIZE;
  g->grille = safeMalloc(g->rows * sizeof(Case *));
  for (int i = 0; i < g->rows; i++) {
    g->grille[i] = safeMalloc(g->columns * sizeof(Case));
  }
  g->history = queue_new();
  g->old_moves = queue_new();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_color(g, i, j, EMPTY);
      game_set_constraint(g, i, j, -1);
    }
  }
  g->wrapping = false;
  g->neigh = FULL;
  return g;
}

// Loren
game game_copy(cgame g) {
  game g1 = game_new_empty_ext(g->rows, g->columns, g->wrapping, g->neigh);
  if ((g != NULL) && (g1 != NULL)) {
    for (int i = 0; i < g->rows; i++) {
      for (int j = 0; j < g->columns; j++) {
        game_set_color(g1, i, j, game_get_color(g, i, j));
        game_set_constraint(g1, i, j, game_get_constraint(g, i, j));
        // game_set_status(g1, i, j);
      }
    }
  } else {
    fprintf(stderr, "game_copy : malloc fail\n");
    exit(EXIT_FAILURE);
  }
  return g1;
}

// Wendy
bool game_equal(cgame g1, cgame g2) {
  if (g1 == NULL || g2 == NULL) {
    fprintf(stderr, "game_equal : invalid parameters\n");
    exit(EXIT_FAILURE);
  }
  if ((g1->rows != g2->rows) || (g1->columns != g2->columns)) return false;
  for (int i = 0; i < g1->rows; i++) {
    for (int j = 0; j < g1->columns; j++) {
      if ((game_get_color(g1, i, j) != game_get_color(g2, i, j)) ||
          (game_get_constraint(g1, i, j) != game_get_constraint(g2, i, j)) ||
          (game_get_status(g1, i, j) != game_get_status(g2, i, j))) {
        return false;
      }
    }
    if ((game_is_wrapping(g1) != game_is_wrapping(g2)) ||
        (game_get_neighbourhood(g1) != game_get_neighbourhood(g2)))
      return false;
  }
  return true;
}

// Wendy
void game_delete(game g) {
  if (g != NULL) {
    if (g->grille != NULL) {
      for (int i = 0; i < g->rows; i++) {
        if (g->grille[i] != NULL) {
          free(g->grille[i]);
        }
      }
      free(g->grille);
    }
    if (g->history != NULL) queue_free_full(g->history, free);
    if (g->old_moves != NULL) queue_free_full(g->old_moves, free);
    free(g);
  }
}

// Wendy
void game_set_constraint(game g, uint i, uint j, constraint n) {
  if ((i >= 0) && (i < g->rows) && (j >= 0) && (j < g->columns)) {
    if ((n >= MIN_CONSTRAINT) && (n <= MAX_CONSTRAINT))
      g->grille[i][j].cons = n;
    else
      fprintf(stderr, "game_set_constraint : invalid constraint\n");
  } else
    printf("game_set_constraint : invalid position\n");
}

// Loren
void game_set_color(game g, uint i, uint j, color c) {
  if ((i >= 0) && (i < g->rows) && (j >= 0) && (j < g->columns))
    g->grille[i][j].col = c;
  else
    fprintf(stderr, "game_set_color : invalid position\n");
}

// Loren
constraint game_get_constraint(cgame g, uint i, uint j) {
  if (((i >= 0) && (i < game_nb_rows(g))) &&
      ((j >= 0) && (j < game_nb_cols(g))))
    return g->grille[i][j].cons;
  else {
    fprintf(stderr, "game_get_constraint : invalid arguments\n");
    exit(EXIT_FAILURE);
  }
}

// Wendy
color game_get_color(cgame g, uint i, uint j) {
  if (((i >= 0) && (i < game_nb_rows(g))) &&
      ((j >= 0) && (j < game_nb_cols(g))))
    return g->grille[i][j].col;
  else {
    fprintf(stderr, "game_get_color : invalid arguments\n");
    exit(EXIT_FAILURE);
  }
}

// Wendy
bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next,
                          uint *pj_next) {
  if (pi_next == NULL || pj_next == NULL) {
    fprintf(stderr, "game_get_next_square : invalid pointeurs\n");
    exit(EXIT_FAILURE);
  }
  // On pourrait faire un dictionnaire avec la direction en clé et une liste des
  // p__next comme valeur et aussi pour neighboors
  bool w = game_is_wrapping(g);
  if (!w) {
    if (!valid_next_square(g, i, j, dir)) {
      return false;
    } else {
      if (dir == UP) {
        *pi_next = i - 1;
        *pj_next = j;
      }
      if (dir == UP_LEFT) {
        *pi_next = i - 1;
        *pj_next = j - 1;
      }
      if (dir == UP_RIGHT) {
        *pi_next = i - 1;
        *pj_next = j + 1;
      }
      if (dir == DOWN) {
        *pi_next = i + 1;
        *pj_next = j;
      }
      if (dir == DOWN_LEFT) {
        *pi_next = i + 1;
        *pj_next = j - 1;
      }
      if (dir == DOWN_RIGHT) {
        *pi_next = i + 1;
        *pj_next = j + 1;
      }
      if (dir == LEFT) {
        *pi_next = i;
        *pj_next = j - 1;
      }
      if (dir == RIGHT) {
        *pi_next = i;
        *pj_next = j + 1;
      }
      if (dir == HERE) {
        *pi_next = i;
        *pj_next = j;
      }
      return true;
    }
  }

  // On pourrait faire un dictionnaire avec la direction en clé et une liste des
  // p__next comme valeur
  else {
    if (dir == UP) {
      if (i == 0) {
        *pi_next = game_nb_rows(g) - 1;
      } else
        *pi_next = i - 1;
      *pj_next = j;
    }
    if (dir == UP_LEFT) {
      if (i == 0) {
        *pi_next = game_nb_rows(g) - 1;
      } else
        *pi_next = i - 1;
      if (j == 0) {
        *pj_next = game_nb_cols(g) - 1;
      } else
        *pj_next = j - 1;
    }
    if (dir == UP_RIGHT) {
      if (i == 0) {
        *pi_next = game_nb_rows(g) - 1;
      } else
        *pi_next = i - 1;
      if (j == game_nb_cols(g) - 1) {
        *pj_next = 0;
      } else
        *pj_next = j + 1;
    }
    if (dir == DOWN) {
      if (i == game_nb_rows(g) - 1) {
        *pi_next = 0;
      } else
        *pi_next = i + 1;
      *pj_next = j;
    }
    if (dir == DOWN_LEFT) {
      if (i == game_nb_rows(g) - 1) {
        *pi_next = 0;
      } else
        *pi_next = i + 1;
      if (j == 0) {
        *pj_next = game_nb_cols(g) - 1;
      } else
        *pj_next = j - 1;
    }
    if (dir == DOWN_RIGHT) {
      if (i == game_nb_rows(g) - 1) {
        *pi_next = 0;
      } else
        *pi_next = i + 1;
      if (j == (game_nb_cols(g) - 1)) {
        *pj_next = 0;
      } else
        *pj_next = j + 1;
    }
    if (dir == LEFT) {
      *pi_next = i;
      if (j == 0) {
        *pj_next = game_nb_cols(g) - 1;
      } else
        *pj_next = j - 1;
    }
    if (dir == RIGHT) {
      *pi_next = i;
      if (j == (game_nb_cols(g) - 1)) {
        *pj_next = 0;
      } else
        *pj_next = j + 1;
    }
    if (dir == HERE) {
      *pi_next = i;
      *pj_next = j;
    }
    return true;
  }

  // fprintf(stderr,"game_get_next_square : invalid arguments\n");
  return false;
}

// wendy
status game_get_status(cgame g, uint i, uint j) {
  if (g == NULL) {
    fprintf(stderr, "Game NULL");
    exit(EXIT_FAILURE);
  }

  else if ((i >= 0) && (i < game_nb_rows(g)) && (j >= 0) && (j < game_nb_cols(g))) {
    int nb_empty = game_nb_neighbors(g, i, j, EMPTY);
    int nb_black = game_nb_neighbors(g, i, j, BLACK);
    int nb_white = game_nb_neighbors(g, i, j, WHITE);
    if (game_get_constraint(g, i, j) == UNCONSTRAINED){
      if (nb_empty == 0)
        return SATISFIED;
      else
        return UNSATISFIED;}

    else {
      if ((nb_black > game_get_constraint(g, i, j)) ||
          (nb_white >
           ((nb_white + nb_black + nb_empty) - game_get_constraint(g, i, j))))
        return ERROR;
      else if (nb_empty != 0)
        return UNSATISFIED;
      /*else if (nb_black < game_get_constraint(g, i, j))
        return UNSATISFIED;
      else if ((game_get_constraint(g, i, j) == 0))
        return UNSATISFIED;*/
      else
        return SATISFIED;
    }
  } else {
    fprintf(stderr, "game_get_status : invalid arguments\n");
    exit(EXIT_FAILURE);
  }
}

// Wendy
int game_nb_neighbors(cgame g, uint i, uint j, color c) {
  if ((i < 0) || (i >= g->rows) || (j < 0) || (j >= g->columns)) {
    fprintf(stderr, "game_nb_neighbors : invalid index\n");
    exit(EXIT_FAILURE);
  }
  // faire un dico pour y mettre des listes et faire le truc de neighboors
  int nb = 0;
  direction list_dir[9] = {UP,      DOWN,     LEFT,      RIGHT,     HERE,
                           UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
  uint i_next = 0;
  uint j_next = 0;
  uint *p_i = &i_next;
  uint *p_j = &j_next;
  neighbourhood n = game_get_neighbourhood(g);
  /*for (int d = 0; d < 9; d++) {
    if (game_get_next_square(g, i, j, list_dir[d], p_i, p_j)) {
      if (game_get_color(g, i_next, j_next) == c)
        nb += 1;
    }
  }*/
  for (int d = 0; d < 9; d++) {
    if (n == FULL) {
      if (game_get_next_square(g, i, j, list_dir[d], p_i, p_j)) {
        if (game_get_color(g, i_next, j_next) == c) nb += 1;
      }
    }
    if ((n == FULL_EXCLUDE) && (d != 4)) {
      if (game_get_next_square(g, i, j, list_dir[d], p_i, p_j)) {
        if (game_get_color(g, i_next, j_next) == c) nb += 1;
      }
    }
    if ((n == ORTHO) && (d < 5)) {
      if (game_get_next_square(g, i, j, list_dir[d], p_i, p_j)) {
        if (game_get_color(g, i_next, j_next) == c) nb += 1;
      }
    }
    if ((n == ORTHO_EXCLUDE) && (d < 4)) {
      if (game_get_next_square(g, i, j, list_dir[d], p_i, p_j)) {
        if (game_get_color(g, i_next, j_next) == c) nb += 1;
      }
    }
  }

  return nb;
}

// Loren
void game_play_move(game g, uint i, uint j, color c) {
  move *m = malloc(sizeof(move));
  if (m == NULL) {
    fprintf(stderr, "game_new_ext : invalid parameters\n");
    exit(EXIT_FAILURE);
  }
  m->i = i;
  m->j = j;
  m->old_col = game_get_color(g, i, j);
  m->new_col = c;

  if (((i >= 0) && (i < g->rows)) && ((j >= 0) && (j < g->columns))) {
    game_set_color(g, i, j, c);
    // game_set_status(g, i, j);
    queue_push_head(g->history, m);
    queue_clear_full(g->old_moves, free);
  } else {
    free(m);
    fprintf(stderr, "game_play_move : invalid index\n");
    exit(EXIT_FAILURE);
  }
}

// Wendy
bool game_won(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "game_won : invalid argument\n");
    exit(EXIT_FAILURE);
  }
  for (uint i = 0; i < g->rows; i++) {
    for (uint j = 0; j < g->columns; j++) {
      if (game_get_color(g, i, j) == EMPTY) return false;
      if (game_get_status(g, i, j) != SATISFIED) return false;
    } 
  }
  return true;
}

// Loren
void game_restart(game g) {
  for (uint i = 0; i < g->rows; i++) {
    for (uint j = 0; j < g->columns; j++) {
      game_set_color(g, i, j, EMPTY);
      // game_set_status(g, i, j);
    }
  }
  queue_free(g->history);
  queue_free(g->old_moves);
  g->history = queue_new();
  g->old_moves = queue_new();
}
