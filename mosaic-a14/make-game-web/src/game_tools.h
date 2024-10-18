/**
 * @file game_tools.h
 * @brief Game Tools.
 * @details See @ref index for further details.
 * @copyright University of Bordeaux. All rights reserved, 2023.
 *
 **/

#ifndef __GAME_TOOLS_H__
#define __GAME_TOOLS_H__
#include <stdbool.h>
#include <stdio.h>

#include "game.h"
#include "game_ext.h"

/**
 * @name Game Tools
 * @{
 */

/**
 * @brief Creates a game by loading its description from a text file.
 * @details See the file format description in @ref index.
 * @param filename input file
 * @return the loaded game
 **/
game game_load(char *filename);

/**
 * @brief Saves a game in a text file.
 * @details See the file format description in @ref index.
 * @param g game to save
 * @param filename output file
 **/
void game_save(cgame g, char *filename);

/**
 * @brief Computes the solution of a given game
 * @param g the game to solve
 * @details The game @p g is updated with the first solution found. If there are
 * no solution for this game, @p g must be unchanged.
 * @return true if a solution is found, false otherwise
 */
bool game_solve(game g);

/**
 * @brief Computes the total number of solutions of a given game.
 * @param g the game
 * @details The game @p g must be unchanged.
 * @return the number of solutions
 */
uint game_nb_solutions(cgame g);

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

/**
 * @}
 */

bool game_has_error(cgame g);

#endif // __GAME_TOOLS_H__
