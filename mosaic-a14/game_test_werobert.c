#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "queue.h"

bool test_dummy(void) { return true; }

bool test_game_default(void) {
  game g = game_default();
  constraint cons1[25] = {0,  -1, -1, 3,  -1, -1, 5,  -1, -1, -1, -1, -1, 4,
                          -1, 1,  6,  -1, 6,  3,  -1, -1, -1, -1, -1, -1};
  game g_new = game_new(cons1, NULL);

  bool test1 = true;
  bool test2 = true;
  bool test3 = true;
  bool test4 = false;
  // test1 : teste si g a été créer correctement
  if (g == NULL) test1 = false;

  // test2 : vérifie si aucune case du jeu n'a été jouer donc sont toutes EMPTY
  // et si constrained -> UNSATISFIED
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_color(g, i, j) != EMPTY) test2 = false;
      if ((game_get_status(g, i, j) != UNSATISFIED) &&
          (game_get_constraint(g, i, j) != UNCONSTRAINED))
        test3 = false;
    }
  }
  if (game_equal(g_new, g)) test4 = true;
  game_delete(g);
  game_delete(g_new);
  return (test1 && test2 && test3 && test4);
}

bool test_game_new_empty(void) {
  game g = game_new_empty();
  bool test1 = true;
  bool test2 = true;
  bool test3 = false;

  // test1 : vérifie que toutes les cases n'ont pas été jouer donc sont EMPTY
  // test2 : vérifie que chaque cases soient UNCONSTRAINED et UNSATISFIED
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_color(g, i, j) != EMPTY) test1 = false;
      if (game_get_constraint(g, i, j) != UNCONSTRAINED) test2 = false;
      if ((game_get_status(g, i, j) != UNSATISFIED) &&
          (game_get_constraint(g, i, j) != UNCONSTRAINED))
        test2 = false;
    }
  }

  // test3
  if ((!game_is_wrapping(g)) && (game_get_neighbourhood(g) == FULL))
    test3 = true;

  game_delete(g);
  return (test1 && test2 && test3);
}

bool test_game_delete(void) {
  // renvoie true si le game_delete ne retourne pas d'erreur
  game g = game_default();
  game_delete(g);
  return true;
}

bool test_game_get_constraint(void) {
  game g = game_new_empty();
  game g_ext = game_new_empty_ext(2, 2, false, FULL);
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;

  // test1 : vérifie si la case a est bien UNCONSTRAINED
  game_set_constraint(g, 0, 0, -1);
  if (game_get_constraint(g, 0, 0) == UNCONSTRAINED) test1 = true;

  // test2 : vérifie si la case a bien une contrainte de 2
  game_set_constraint(g, 0, 0, 2);
  if (game_get_constraint(g, 0, 0) == 2) test2 = true;

  game_set_constraint(g_ext, 0, 0, -1);
  if (game_get_constraint(g_ext, 0, 0) == UNCONSTRAINED) test3 = true;

  game_set_constraint(g_ext, 0, 0, 2);
  if (game_get_constraint(g_ext, 0, 0) == 2) test3 = true;

  game_delete(g);
  game_delete(g_ext);

  return (test1 && test2 && test3);
}

bool test_game_won() {
  game g_not_won = game_default();
  game g_won = game_default_solution();
  game g = game_new_empty();
  game g_ext = game_new_empty_ext(2, 2, false, FULL);

  // test1 : vérifie si game_won avec comme argument g_not_won, qui n'est pas un
  // jeu gagné, renvoie bien false
  bool test1 = !game_won(g_not_won);

  // test2 : verifie si game_won avec comme argument g_won renvoie true
  bool test2 = game_won(g_won);

  for (int i = 0; i < game_nb_rows(g_won); i++) {
    for (int j = 0; j < game_nb_cols(g_won); j++) {
      if (game_get_status(g_won, i, j) != SATISFIED) return false;
    }
  }

  bool test3 = !game_won(g);

  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      game_set_color(g, i, j, WHITE);
    }
  }

  bool test4 = game_won(g);

  bool test5 = !game_won(g_ext);

  game_delete(g_not_won);
  game_delete(g_won);
  game_delete(g);
  game_delete(g_ext);
  return (test1 && test2 && test3 && test4 && test5);
}

bool test_game_get_status() {
  game g = game_default_solution();
  game g2 = game_new_empty_ext(5, 5, true, ORTHO);
  bool test1 = true;
  bool test2 = false;
  bool test3 = false;

  game_set_constraint(g2, 2, 2, UNCONSTRAINED);
  game_set_color(g2, 2, 2, WHITE);
  bool test4 = (game_get_status(g2, 2, 2) == UNSATISFIED);
  game_set_color(g2, 1, 2, BLACK);
  game_set_color(g2, 2, 1, BLACK);
  game_set_color(g2, 2, 3, BLACK);
  bool test5 = (game_get_status(g2, 2, 2) == UNSATISFIED);
  game_set_color(g2, 3, 2, BLACK);
  // game_set_color(g2, 2, 2, BLACK);
  bool test6 = (game_get_status(g2, 2, 2) == SATISFIED);  // false

  game_set_constraint(g2, 2, 2, 5);
  game_set_color(g2, 2, 2, EMPTY);
  bool test7 = (game_get_status(g2, 2, 2) == UNSATISFIED);
  game_set_color(g2, 2, 2, BLACK);
  bool test8 = (game_get_status(g2, 2, 2) == SATISFIED);  // false
  game_set_color(g2, 2, 2, WHITE);
  bool test9 = (game_get_status(g2, 2, 2) == ERROR);

  // test1 : vérifie si chaque case est satisfaite dans un jeu gagné
  for (int i = 0; i < DEFAULT_SIZE - 1; i++) {
    for (int j = 0; j < DEFAULT_SIZE - 1; j++) {
      if (game_get_status(g, i, j) != SATISFIED) test1 = false;
    }
  }

  // test2 : modifie la case (0,0) en BLACK et vérifie si le status de cette
  // case est bien UNSATISFIED
  game_play_move(g, 0, 0, EMPTY);
  if (game_get_status(g, 0, 0) == UNSATISFIED) test2 = true;

  // test3 : modifie les cases (0,0) et (0,1) de façon a obtenir une erreur
  game_play_move(g, 0, 0, BLACK);
  game_play_move(g, 0, 1, BLACK);
  if (game_get_status(g, 0, 0) == ERROR) test3 = true;

  game_delete(g);
  game_delete(g2);

  return (test1 && test2 && test3 && test4 && test5 && test6 && test7 &&
          test8 && test9);
}

bool test_game_default_solution() {
  game g = game_default_solution();
  game g_default = game_default();

  // test1 : teste si game_default_solution est gagné
  // bool test1 = game_won(g);
  bool test1 = true;
  bool test2 = true;
  bool test3 = false;

  color solution[25] = {WHITE, WHITE, BLACK, WHITE, WHITE, WHITE, WHITE,
                        BLACK, WHITE, BLACK, BLACK, BLACK, BLACK, WHITE,
                        WHITE, BLACK, BLACK, WHITE, WHITE, WHITE, BLACK,
                        BLACK, BLACK, BLACK, WHITE};

  // test2 : vérifie si la solution a bien les même contraintes que game_default
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      game_play_move(g_default, i, j, solution[i * 5 + j]);
      if (game_get_constraint(g, i, j) != game_get_constraint(g_default, i, j))
        test2 = false;
    }
  }
  if (game_equal(g_default, g)) test3 = true;
  game_delete(g_default);
  game_delete(g);
  return (test1 && test2 && test3);
}

bool test_game_copy() {
  game g = game_default();
  game g_copy = game_copy(g);

  bool test1 = game_equal(g, g_copy);
  bool test2 = (game_get_neighbourhood(g) == game_get_neighbourhood(g_copy));
  bool test3 = (game_nb_rows(g) == game_nb_rows(g_copy));
  bool test4 = (game_nb_cols(g) == game_nb_cols(g_copy));
  bool test5 = (game_is_wrapping(g) == game_is_wrapping(g_copy));
  game_delete(g);
  game_delete(g_copy);
  return (test1 && test2 && test3 && test4 && test5);
}

bool test_game_set_constraint() {
  game g = game_default();
  bool test1 = false;
  bool test2 = false;

  // test1 : vérifie que la case est UNCONSTRAINED
  game_set_constraint(g, 0, 0, UNCONSTRAINED);
  if (game_get_constraint(g, 0, 0) == UNCONSTRAINED) test1 = true;

  // test2 : vérie que la contrainte est 2
  game_set_constraint(g, 0, 1, 2);
  if (game_get_constraint(g, 0, 1) == 2) test2 = true;

  game_delete(g);

  return (test1 && test2);
}

bool test_game_get_color() {
  game g = game_default();
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;

  // test1
  game_set_color(g, 0, 0, WHITE);
  if (game_get_color(g, 0, 0) == WHITE) test1 = true;

  // test2
  game_set_color(g, 0, 0, BLACK);
  if (game_get_color(g, 0, 0) == BLACK) test2 = true;

  // test3
  game_set_color(g, 0, 0, EMPTY);
  if (game_get_color(g, 0, 0) == EMPTY) test3 = true;

  game_delete(g);

  // resultats
  return (test1 && test2 && test3);
}

bool test_game_nb_neighbors() {
  game g = game_default();
  game g_ortho = game_new_empty_ext(5, 5, false, ORTHO);
  game g_f_ex = game_new_empty_ext(5, 5, false, FULL_EXCLUDE);
  game g_ext = game_new_empty_ext(2, 2, false, FULL);
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;
  bool test4 = false;
  bool test5 = false;
  bool test6 = false;
  bool test7 = false;

  // on met toutes les cases en BLACK dans le caré (0,0) à (2,2)
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      game_set_color(g, i, j, BLACK);
    }
  }

  // test1 : vérifie que les nombre de cases BLACK est 9 car elles sont toutes
  // de la même couleur
  if (game_nb_neighbors(g, 1, 1, BLACK) == 9) test1 = true;

  // test2 : on vérifie qu'il n'y a aucune case WHITE
  if (game_nb_neighbors(g, 1, 1, WHITE) == 0) test2 = true;

  // test3 : le nombre de cases EMPTY autour de la case (3,3) est 8 car (2,2)
  // est BLACK
  if (game_nb_neighbors(g, 3, 3, EMPTY) == 8) test3 = true;

  // test4 : il n'y a aucune case BLACK autour de (4,4)
  if (game_nb_neighbors(g, 4, 4, BLACK) == 0) test4 = true;

  if (game_nb_neighbors(g_ortho, 1, 0, EMPTY) == 4) test5 = true;

  if (game_nb_neighbors(g_f_ex, 0, 0, EMPTY) == 3) test6 = true;

  if (game_nb_neighbors(g_ext, 0, 0, EMPTY) == 4) test7 = true;

  game_delete(g);
  game_delete(g_ortho);
  game_delete(g_f_ex);
  game_delete(g_ext);

  return (test1 && test2 && test3 && test4 && test5 && test6 && test7);
}

bool test_game_restart() {
  game g = game_default_solution();
  bool test = true;
  game_restart(g);
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_color(g, i, j) != EMPTY) test = false;
    }
  }

  game_delete(g);

  return test;
}

bool test_game_new_ext(void) {
  constraint cons1[25] = {0,  -1, -1, 3,  -1, -1, 5,  -1, -1, -1, -1, -1, 4,
                          -1, 1,  6,  -1, 6,  3,  -1, -1, -1, -1, -1, -1};
  constraint cons2[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
  color col[9] = {BLACK, BLACK, BLACK, BLACK, BLACK,
                  BLACK, BLACK, BLACK, BLACK};
  game g = game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, cons1, NULL, false, FULL);
  game g1 = game_new_ext(3, 3, cons2, col, true, ORTHO);
  game g2 = game_default();
  bool test1 = false;
  bool test2 = true;
  bool test3 = true;
  bool test4 = true;
  bool test5 = false;
  if (g == NULL) return false;
  if (g1 == NULL) return false;

  if (game_equal(g, g2)) test1 = true;
  for (int i = 0; i < game_nb_rows(g1); i++) {
    for (int j = 0; j < game_nb_cols(g1); j++) {
      if (game_get_color(g1, i, j) != BLACK) test2 = false;
      if (game_get_constraint(g1, i, j) != -1) test3 = false;
    }
  }
  if (game_is_wrapping(g1) == true) test5 = true;
  if (game_get_neighbourhood(g1) != ORTHO) test4 = false;
  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  return (test1 && test2 && test3 && test4 && test5);
}

bool test_game_new_empty_ext(void) {
  game g = game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false, FULL);
  game g1 = game_new_empty_ext(3, 3, true, ORTHO);
  bool test1 = true;
  bool test2 = true;
  bool test3 = false;

  if ((g == NULL) || (g1 == NULL)) return false;

  // test1 : vérifie que toutes les cases n'ont pas été jouer donc sont EMPTY
  // test2 : vérifie que chaque cases soient UNCONSTRAINED et UNSATISFIED
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_color(g, i, j) != EMPTY) test1 = false;
      if (game_get_constraint(g, i, j) != UNCONSTRAINED) test2 = false;
      if ((game_get_status(g, i, j) != UNSATISFIED) &&
          (game_get_constraint(g, i, j) != UNCONSTRAINED))
        test2 = false;
    }
  }

  // test3
  if ((!game_is_wrapping(g)) && (game_get_neighbourhood(g) == FULL))
    test3 = true;
  if ((game_is_wrapping(g1)) && (game_get_neighbourhood(g) == ORTHO))
    test3 = true;

  game_delete(g);
  game_delete(g1);
  return (test1 && test2 && test3);
}

bool test_game_undo(void) {
  bool test1 = false;
  game g = game_new_empty();
  game_play_move(g, 0, 0, WHITE);
  game_undo(g);
  if (game_get_color(g, 0, 0) == EMPTY) test1 = true;
  game_delete(g);
  return test1;
}

bool test_game_redo(void) {
  bool test1 = false;
  game g = game_new_empty();
  game_play_move(g, 0, 0, WHITE);
  game_undo(g);
  game_redo(g);
  if (game_get_color(g, 0, 0) == WHITE) test1 = true;
  game_delete(g);
  return test1;
}

bool test_game_save() {
  game g_default = game_default();
  game g_ext = game_new_empty_ext(4, 6, true, ORTHO_EXCLUDE);
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;
  bool test4 = false;
  bool test5 = false;

  game_save(g_default, "save.txt");
  game_save(g_ext, "save_ext.txt");
  FILE *f = fopen("save.txt", "r");

  int nb_rows;
  int nb_cols;
  int wrap;
  int neigh;

  fscanf(f, "%d %d %d %d", &nb_rows, &nb_cols, &wrap, &neigh);
  if ((nb_rows == game_nb_rows(g_default)) &&
      (nb_cols == game_nb_cols(g_default)))
    test1 = true;

  if (wrap == 1) {
    if (game_is_wrapping(g_default)) test2 = true;
  } else {
    if (!game_is_wrapping(g_default)) test2 = true;
  }

  neighbourhood tab_dir[4] = {FULL, FULL_EXCLUDE, ORTHO, ORTHO_EXCLUDE};
  if (tab_dir[neigh] == game_get_neighbourhood(g_default)) test3 = true;
  char cons;
  char color;
  for (int i = 0; i < game_nb_rows(g_default); i++) {
    for (int j = 0; j < game_nb_cols(g_default); j++) {
      fscanf(f, "%c%c", &cons, &color);

      if (cons == '-') {
        if (game_get_constraint(g_default, i, j) == UNCONSTRAINED) test4 = true;
      } else {
        if (game_get_constraint(g_default, i, j) == cons) test4 = true;
      }

      if (color == 'e') {
        if (game_get_color(g_default, i, j) == EMPTY) test5 = true;
      }
      if (color == 'w') {
        if (game_get_color(g_default, i, j) == WHITE) test5 = true;
      }
      if (color == 'b') {
        if (game_get_color(g_default, i, j) == BLACK) test5 = true;
      }
    }
    fscanf(f, "\n");
  }
  fclose(f);
  game_delete(g_default);
  game_delete(g_ext);
  return test1 && test2 && test3 && test4 && test5;
}

bool test_game_nb_solutions(){
  game g_ext = game_new_empty_ext(2,2,true,FULL);
  game g_default = game_default();

  bool test1 = false;
  bool test2 = false;
  bool test3 = false;
  
  if (game_nb_solutions(g_ext)>0) test1 = true;

  game_set_constraint(g_ext,0,0,4);
  game_set_constraint(g_ext,0,1,0);
  if (game_nb_solutions(g_ext)==0) test2 = true;

  if (game_nb_solutions(g_default)==1) test3 = true;

  game_delete(g_ext);
  game_delete(g_default);

  return test1&&test2&&test3;
}

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;

  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else if (strcmp("game_default", argv[1]) == 0)
    ok = test_game_default();
  else if (strcmp("game_new_empty", argv[1]) == 0)
    ok = test_game_new_empty();
  else if (strcmp("game_delete", argv[1]) == 0)
    ok = test_game_delete();
  else if (strcmp("game_get_constraint", argv[1]) == 0)
    ok = test_game_get_constraint();
  else if (strcmp("game_won", argv[1]) == 0)
    ok = test_game_won();
  else if (strcmp("game_get_status", argv[1]) == 0)
    ok = test_game_get_status();

  else if (strcmp("game_default_solution", argv[1]) == 0)
    ok = test_game_default_solution();
  else if (strcmp("game_copy", argv[1]) == 0)
    ok = test_game_copy();
  else if (strcmp("game_set_constraint", argv[1]) == 0)
    ok = test_game_set_constraint();
  else if (strcmp("game_get_color", argv[1]) == 0)
    ok = test_game_get_color();
  else if (strcmp("game_nb_neighbors", argv[1]) == 0)
    ok = test_game_nb_neighbors();
  else if (strcmp("game_restart", argv[1]) == 0)
    ok = test_game_restart();

  else if (strcmp("game_new_ext", argv[1]) == 0)
    ok = test_game_new_ext();
  else if (strcmp("game_new_empty_ext", argv[1]) == 0)
    ok = test_game_new_empty_ext();
  else if (strcmp("game_undo", argv[1]) == 0)
    ok = test_game_undo();
  else if (strcmp("game_redo", argv[1]) == 0)
    ok = test_game_redo();

  else if (strcmp("game_save", argv[1]) == 0)
    ok = test_game_save();

  else if (strcmp("game_nb_solutions", argv[1]) == 0)
    ok = test_game_nb_solutions();

  else {
    fprintf(stderr, "Error: test failure\n");
    return EXIT_FAILURE;
  }

  if (ok == true) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}