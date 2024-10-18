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

// test : verifie si ça que le fonnction affiche bien le jeu
bool test_game_print(void) {
  game g = game_new_empty_ext(1, 1, false, FULL);
  game_print(g);
  game_delete(g);
  return true;
}

// test1 : verifie que la couleur WHITE est valide
// test2 : verifie que la couleur BLACK est valide
// test3 : verifie que la couleur EMPTY est valide
bool test_game_set_color(void) {
  game g = game_default();
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;

  // test1
  game_set_color(g, 1, 2, WHITE);
  if (game_get_color(g, 1, 2) == WHITE) test1 = true;

  // test1
  game_set_color(g, 1, 2, BLACK);
  if (game_get_color(g, 1, 2) == BLACK) test2 = true;

  // test1
  game_set_color(g, 1, 2, EMPTY);
  if (game_get_color(g, 1, 2) == EMPTY) test3 = true;

  game_delete(g);
  return (test1 && test2) && test3;
}

// test1 : verifie que pour le nouveau jeu on a game_new() avec les paramètre de
// game_default soit égal à game_default() test2 : verifie que toutes les cases
// du tableau sont bien BLACK test3 : verifie qu'il n'y a pas de contraintes
// dans chaque case

// test4 ; verifie que tous les neighbourhood de chaque cases sont FULL;

bool test_game_new(void) {
  constraint cons1[25] = {0,  -1, -1, 3,  -1, -1, 5,  -1, -1, -1, -1, -1, 4,
                          -1, 1,  6,  -1, 6,  3,  -1, -1, -1, -1, -1, -1};
  constraint cons2[25] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                          -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  color col[25] = {BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
                   BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
                   BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
                   BLACK, BLACK, BLACK, BLACK};
  game g;
  game g1;
  // neighbourhood neigh = FULL;
  bool test1 = false;
  bool test2 = true;
  bool test3 = true;
  bool test4 = true;
  bool test5 = false;
  g = game_new(cons1, NULL);
  g1 = game_new(cons2, col);
  if (g == NULL) return false;
  game g2 = game_default();
  if (game_equal(g, g2)) test1 = true;
  for (int i = 0; i < game_nb_rows(g1); i++) {
    for (int j = 0; j < game_nb_cols(g1); j++) {
      if (game_get_color(g1, i, j) != BLACK) test2 = false;
      if (game_get_constraint(g1, i, j) != -1) test3 = false;
    }
  }
  if (game_is_wrapping(g1) == false) test5 = true;
  if (game_get_neighbourhood(g1) != FULL) test4 = false;
  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  return (test1 && test2) && ((test3 && test4) && test5);
}

// test : verifie que deux game_default() sont égaux
// test2 : verifie que gma_default() et game_new_empty() sont égaux

bool test_game_equal() {
  game g1 = game_default();
  game g2 = game_default();
  game g3 = game_new_empty();
  bool test = game_equal(g1, g2);
  bool test2 = !game_equal(g1, g3);

  if ((game_get_neighbourhood(g1) != game_get_neighbourhood(g2)) ||
      (game_nb_cols(g1) != game_nb_cols(g2)) ||
      (game_nb_rows(g1) != game_nb_rows(g2))) {
    return false;
  }

  for (int i = 0; i < game_nb_rows(g1); i++) {
    for (int j = 0; j < game_nb_cols(g1); j++) {
      if (game_get_color(g1, i, j) != game_get_color(g2, i, j)) return false;
      if (game_get_constraint(g1, i, j) != game_get_constraint(g2, i, j))
        return false;
      if (game_get_status(g1, i, j) != game_get_status(g2, i, j)) return false;
    }
  }

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return (test && test2);
}

// test1,2,3,4 : verifie que chaque direction et valide.

bool test_game_get_next_square(void) {
  game g = game_default();
  game g_wrap = game_new_empty_ext(5, 5, true, ORTHO);

  uint a = 2;
  uint b = 2;
  uint *pi_next = &a;
  uint *pj_next = &b;
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;
  bool test4 = false;
  bool test5 = false;

  // test1
  if (game_get_next_square(g, 2, 2, RIGHT, pi_next, pj_next)) {
    if (a == 2) {
      if (b == 3) test1 = true;
    }
  }

  // test2
  if (game_get_next_square(g, 2, 2, LEFT, pi_next, pj_next)) {
    if (a == 2) {
      if (b == 1) test2 = true;
    }
  }

  // test3
  if (game_get_next_square(g, 2, 2, UP, pi_next, pj_next)) {
    if (a == 1) {
      if (b == 2) test3 = true;
    }
  }

  // test4
  if (game_get_next_square(g, 2, 2, DOWN, pi_next, pj_next)) {
    if (a == 3) {
      if (b == 2) test4 = true;
    }
  }

  if (game_get_next_square(g_wrap, 4, 0, DOWN, pi_next, pj_next)) {
    if (a == 0) {
      if (b == 0) test5 = true;
    }
  }

  game_delete(g);
  game_delete(g_wrap);
  return ((test1 && test2) && (test3 && test4 && test5));
}

// test1,2,3 : verifie que par exemple si je suis dans la case (2,2) de couleur
// WHITE, le test renvoi true si après verification la case (2,2) est bien WHITE

bool test_game_play_move(void) {
  game g = game_default();
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;

  // test1
  game_play_move(g, 2, 2, WHITE);
  if (game_get_color(g, 2, 2) == WHITE) test1 = true;

  // test2
  game_play_move(g, 2, 2, BLACK);
  if (game_get_color(g, 2, 2) == BLACK) test2 = true;

  // test3
  game_play_move(g, 2, 2, EMPTY);
  if (game_get_color(g, 2, 2) == EMPTY) test3 = true;

  game_delete(g);
  return (test1 && test2) && test3;
}

// verifie que la colonne la plus à droite est adjacente à la colonne plus à
// gauche et la ligne la plus haute est adjacente à la ligne la plus basse
bool test_game_is_wrapping() {
  bool test = false;
  game g = game_new_empty_ext(1, 1, false, FULL);
  game g1 = game_new_empty_ext(1, 1, true, FULL);
  if ((game_is_wrapping(g) == false) && (game_is_wrapping(g1) == true)) {
    test = true;
  }
  game_delete(g);
  game_delete(g1);
  return test;
}

// test 1 et 2 : verifie que le nombre de ligne du jeu correspond bien au nombre
// de ligne du nouveau jeu
bool test_game_nb_rows(void) {
  bool test1 = false;
  bool test2 = false;
  game g1 = game_new_empty_ext(5, 2, false, FULL);
  game g2 = game_new_empty_ext(4, 3, true, FULL);
  if (game_nb_rows(g1) == 5) {
    test1 = true;
  }
  if ((game_nb_rows(g2) == 4)) {
    test2 = true;
  }
  game_delete(g1);
  game_delete(g2);
  return test1 && test2;
}

// test 1 et 2 : verifie que le nombre de colonne du jeu correspond bien au
// nombre de colonne du nouveau jeu
bool test_game_nb_cols(void) {
  bool test1 = false;
  bool test2 = false;
  game g1 = game_new_empty_ext(5, 2, false, FULL);
  game g2 = game_new_empty_ext(4, 3, true, FULL);
  if (game_nb_cols(g1) == 2) {
    test1 = true;
  }
  if ((game_nb_cols(g2) == 3)) {
    test2 = true;
  }
  game_delete(g1);
  game_delete(g2);
  return test1 && test2;
}

bool test_game_get_neighbourhood() {
  bool test = false;
  game g = game_new_empty_ext(1, 1, false, FULL);
  game g1 = game_new_empty_ext(1, 1, false, ORTHO);
  if ((game_get_neighbourhood(g) == FULL) &&
      (game_get_neighbourhood(g1) == ORTHO)) {
    test = true;
  }
  game_delete(g);
  game_delete(g1);
  return test;
}

bool test_game_load() {
  game g = game_default();
  game g1 = game_load("default.txt");
  game g2 = game_load("solution.txt");
  game g3 = game_new_empty_ext(2, 5, true, ORTHO);
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;

  game_save(g3, "ext.txt");
  game g3_load = game_load("ext.txt");

  if (game_equal(g1, g)) {
    test1 = true;
  }
  if (game_won(g2)) {
    test2 = true;
  }
  if (game_equal(g3, g3_load)) {
    test3 = true;
  }

  game_delete(g);
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g3_load);
  return test1 && test2 && test3;
}

bool test_game_solve(){
  game g = game_new_empty_ext(4,4,true,FULL);
  game g_default = game_default();


  bool test = false;
  bool test1 = false;
  bool test2 = false;
  bool test3 = false;

  if((game_solve(g))&&(game_won(g))){
    test = true;
  }
  game_set_constraint(g,1,1,1);
  game_set_constraint(g,0,0,3);
  game_set_constraint(g,0,game_nb_cols(g)-1,0);

  game g_copy = game_copy(g);
  game_restart(g_copy);
  game_restart(g);
  if(!game_solve(g)){
    if (game_equal(g_copy,g)) test1 = true;
  }

  game_set_constraint(g,0,0,UNCONSTRAINED);
  game_restart(g);
  if (game_solve(g)){
    test2 = true;
  }

  if (game_solve(g_default)){
    test3 = true;
  }

  game_delete(g);
  game_delete(g_copy);
  game_delete(g_default);

  return test && test1 && test2 && test3;
}


void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else if (strcmp("game_print", argv[1]) == 0)
    ok = test_game_print();
  else if (strcmp("game_new", argv[1]) == 0)
    ok = test_game_new();
  else if (strcmp("game_equal", argv[1]) == 0)
    ok = test_game_equal();
  else if (strcmp("game_set_color", argv[1]) == 0)
    ok = test_game_set_color();
  else if (strcmp("game_get_next_square", argv[1]) == 0)
    ok = test_game_get_next_square();
  else if (strcmp("game_play_move", argv[1]) == 0)
    ok = test_game_play_move();

  else if (strcmp("game_nb_cols", argv[1]) == 0)
    ok = test_game_nb_cols();
  else if (strcmp("game_nb_rows", argv[1]) == 0)
    ok = test_game_nb_rows();
  else if (strcmp("game_is_wrapping", argv[1]) == 0)
    ok = test_game_is_wrapping();
  else if (strcmp("game_get_neighbourhood", argv[1]) == 0)
    ok = test_game_get_neighbourhood();
  else if (strcmp("game_load", argv[1]) == 0)
    ok = test_game_load();
  else if (strcmp("game_solve", argv[1]) == 0)
    ok = test_game_solve();
  if (ok == true) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
