#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  game g;

  if (argc == 2) {
    g = game_load(argv[1]);
    if (g == NULL) {
      fprintf(stderr, "game_text : game_load fail");
    }
  } else
    g = game_default();

  char c;
  int i, j;

  if (g == NULL) return EXIT_FAILURE;
  printf(
      "h : aide\nr : reset\nq : quitter\nw : white\nb : black\ne : empty\nz : "
      "undo\ny : redo\n");

  while ((game_won(g) == false) || (g != NULL)) {
    game_print(g);

    // Jeu fini
    if (game_won(g)) {
      printf("congratulation\n");
      game_delete(g);
      return EXIT_SUCCESS;
    }

    // Liste d'erreurs
    for (int a = 0; a < game_nb_rows(g); a++) {
      for (int b = 0; b < game_nb_cols(g); b++) {
        if (game_get_status(g, a, b) == ERROR) {
          printf("Erreur case (%d,%d)\n", a, b);
        }
      }
    }

    // Lire le caractère en entrée standard
    printf("Qu'est ce que vous voulez faire ?\nSaisir un caractère : ");
    scanf(" %c", &c);

    // Affiche l'aide
    if (c == 'h')
      printf(
          "h : aide\nr : reset\nq : quitter\nw : white\nb : black\ne : "
          "empty\nz : undo\ny : redo\ns : save\n");

    // Réinitialise la grille
    else if (c == 'r')
      game_restart(g);

    // Quitte
    else if (c == 'q') {
      game_delete(g);
      printf("shame\n");
      return EXIT_SUCCESS;
    }

    // Changer une case
    else if (((c == 'w') || (c == 'b')) || (c == 'e')) {
      printf("Veuillez saisir deux entiers : ");
      scanf("%d %d", &i, &j);
      if ((i < game_nb_rows(g)) && (j < game_nb_cols(g))) {
        if (c == 'w')
          game_play_move(g, i, j, WHITE);
        else if (c == 'b')
          game_play_move(g, i, j, BLACK);
        else if (c == 'e')
          game_play_move(g, i, j, EMPTY);
      }
    }

    else if (c == 'z') {
      game_undo(g);
      printf("Le mouvement précédent a été annulé\n");
    }

    else if (c == 'y') {
      game_redo(g);
      printf("Le mouvement précédemment annulé a été rejouer\n");
    }

    else if (c == 's') {
      char nameFile;
      printf("Veuillez saisir un nom de sauvegarde : ");
      scanf("%s", &nameFile);
      game_save(g, &nameFile);
    }
  }
  game_delete(g);
  return EXIT_SUCCESS;
}
