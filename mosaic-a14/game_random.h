#ifndef __GAME_RANDOM_H__
#define __GAME_RANDOM_H__

#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "game_ext.h"

/**
 * Create a random game with a given size and options.
 *
 * @param nb_rows the number of rows of the game
 * @param nb_cols the number of columns of the game
 * @param wrapping wrapping option
 * @param neigh neighbourhood option
 * @param with_solution if true, the game contains the solution
 * @param black_rate the rate of black squares
 * @param constraint_rate the rate of constrained squares
 * @pre @p black_rate must be between 0.0 and 1.0
 * @pre @p constraint_rate must be between 0.0 and 1.0
 *
 * @return the generated random game (or NULL in case of error)
 */
game game_random(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh,
                 bool with_solution, float black_rate, float constraint_rate);

#endif