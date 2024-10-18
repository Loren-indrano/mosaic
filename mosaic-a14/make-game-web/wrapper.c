/**
 * @file wrapper.js
 * @brief Game Binding to JavaScript (based on Emscripten & Wasm)
 * @author aurelien.esnard@u-bordeaux.fr
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#include <emscripten.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* ******************** Game V1 & V2 API ******************** */

EMSCRIPTEN_KEEPALIVE
game new_default(void) { return game_default(); }

EMSCRIPTEN_KEEPALIVE
void delete (game g) { game_delete(g); }

EMSCRIPTEN_KEEPALIVE
color get_color(cgame g, uint i, uint j) { return game_get_color(g, i, j); }

EMSCRIPTEN_KEEPALIVE
constraint get_constraint(cgame g, uint i, uint j) { return game_get_constraint(g, i, j); }

EMSCRIPTEN_KEEPALIVE
void play_move(game g, uint i, uint j, color c) { game_play_move(g, i, j, c); }

EMSCRIPTEN_KEEPALIVE
void restart(game g) { game_restart(g); }

EMSCRIPTEN_KEEPALIVE
bool won(game g) { return game_won(g); }

EMSCRIPTEN_KEEPALIVE
uint nb_rows(cgame g) { return game_nb_rows(g); }

EMSCRIPTEN_KEEPALIVE
uint nb_cols(cgame g) { return game_nb_cols(g); }

EMSCRIPTEN_KEEPALIVE
status get_status(cgame g, uint i, uint j) { return game_get_status(g, i, j); }

EMSCRIPTEN_KEEPALIVE
void undo(game g) { game_undo(g); }

EMSCRIPTEN_KEEPALIVE
void redo(game g) { game_redo(g); }

/* ******************** Game Tools API ******************** */

EMSCRIPTEN_KEEPALIVE
bool solve(game g) { return game_solve(g); }

EMSCRIPTEN_KEEPALIVE
uint nb_solutions(cgame g) { return game_nb_solutions(g); }

EMSCRIPTEN_KEEPALIVE
game new_random(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh,
                float black_rate, float constraint_rate)
{
    srand(time(NULL)); // random seed
    return game_random(nb_rows, nb_cols, wrapping, neigh, false, black_rate, constraint_rate);
}

// EOF
