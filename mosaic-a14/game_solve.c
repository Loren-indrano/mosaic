#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "queue.h"

int main(int argc, char *argv[]) {
    if (argc<3) {
        fprintf(stderr, "game_solve : error arguments");
    }

    char option = argv[1][1]; //juste le caractere
    game g = game_load(argv[2]);
    if (g == NULL) {
      fprintf(stderr, "game_text : game_load fail");
    }

    if (option=='s'){
        if (game_solve(g)){
            if (argc==4){
                FILE * f = fopen(argv[3],"w");
                game_save(g,argv[3]);
                fclose(f);
            }
            else game_print(g);
        }
        else return EXIT_FAILURE;
    }

    else if (option=='c'){
        int nb_sol = game_nb_solutions(g);
        if (argc==4){
            FILE * f = fopen(argv[3],"w");
            fprintf(f,"%d",nb_sol);
            fclose(f);
        }
        else printf("%d",nb_sol);
    }

    return EXIT_SUCCESS;
}