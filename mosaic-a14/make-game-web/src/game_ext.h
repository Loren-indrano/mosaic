/**
 * @file game_ext.h
 * @brief Extended Game Functions.
 * @details See @ref index for further details.
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#ifndef __GAME_EXT_H__
#define __GAME_EXT_H__

#include <stdbool.h>

#include "game.h"

/**
 * @brief The different kind of neighbourhood.
 */
typedef enum {
  FULL,         /**< All neighbours are considered. */
  ORTHO,        /**< Only the orthogonal neighbours are considered. */
  FULL_EXCLUDE, /**< All neighbours are considered, except for the central
                   square. */
  ORTHO_EXCLUDE /**< Only the orthogonal neighbours are considered, except for
                   the central square. */
} neighbourhood;

/**
 * @name Extended Functions
 * @{
 */

/**
 * @brief Creates a new game with extended options and initializes it.
 * @param nb_rows number of rows in game
 * @param nb_cols number of columns in game
 * @param constraints an array describing the initial constraint of each square
 * using row-major storage (of size nb_rows*nb_cols)
 * @param colors an array describing the initial color of each square using
 * row-major storage  (of size nb_rows*nb_cols) or NULL to set all colors as
 * EMPTY
 * @param wrapping wrapping option
 * @param neigh neighbourhood option
 * @pre @p constraints must be an initialized array of default size squared
 * @pre @p colors must be an initialized array of default size squared or NULL
 * @return the created game
 **/
game game_new_ext(uint nb_rows, uint nb_cols, constraint* constraints,
                  color* colors, bool wrapping, neighbourhood neigh);

/**
 * @brief Creates a new empty game with extended options.
 * @details All squares are initialized with empty squares.
 * @param nb_rows number of rows in game
 * @param nb_cols number of columns in game
 * @param wrapping wrapping option
 * @param neigh neighbourhood option
 * @return the created game
 **/
game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        neighbourhood neigh);

/**
 * @brief Gets the number of rows (or height).
 * @param g the game
 * @return the number of rows on this game
 * @pre @p g is a valid pointer toward a cgame structure
 **/
uint game_nb_rows(cgame g);

/**
 * @brief Gets the number of columns (or width).
 * @param g the game
 * @return the the number of columns on this game
 * @pre @p g is a valid pointer toward a cgame structure
 **/
uint game_nb_cols(cgame g);

/**
 * @brief Checks if the game has the wrapping option
 * @return true, if wrapping option is enabled, false otherwise
 * @pre @p g is a valid pointer toward a cgame structure
 **/
bool game_is_wrapping(cgame g);

/**
 * @brief Gets the neighbourhood option
 * @return the neighbourhood option
 * @pre @p g is a valid pointer toward a cgame structure
 **/
neighbourhood game_get_neighbourhood(cgame g);

/**
 * @brief Undoes the last move.
 * @details Searches in the history the last move played (by calling
 * @ref game_play_move or @ref game_redo), and restores the state of the game
 * before that move. If no moves have been played, this function does nothing.
 * The @ref game_restart function clears the history.
 * @param g the game
 * @pre @p g is a valid pointer toward a cgame structure
 **/
void game_undo(game g);

/**
 * @brief Redoes the last move.
 * @details Searches in the history the last cancelled move (by calling @ref
 * game_undo), and replays it. If there are no more moves to be replayed, this
 * function does nothing. After playing a new move with @ref game_play_move, it
 * is no longer possible to redo an old cancelled move.
 * @param g the game
 * @pre @p g is a valid pointer toward a cgame structure
 **/
void game_redo(game g);

/**
 * @}
 */

#endif  // __GAME_EXT_H__
