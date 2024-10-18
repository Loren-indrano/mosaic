/**
 * @file game.h
 * @brief Basic Game Functions.
 * @details See @ref index for further details.
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

/**
 * @brief Standard unsigned integer type.
 **/
typedef unsigned int uint;

/**
 * @brief Size of the default game grid.
 **/
#define DEFAULT_SIZE 5

/** square colors */
typedef enum
{
  EMPTY, /**< empty square */
  WHITE, /**< white square */
  BLACK  /**< blacksquare */
} color;

/** square constraints
 * @details These constants are numerical constraints that indicate the expected
 * number of black squares in the 3x3 neighborhood of a given square (including
 * itself). All valid constant numbers are between 0 and 9. The special constant
 * UNCONSTRAINED (-1) indicates that there is no particular constraint on a
 * square, i.e. there can be any number of black squares in its neighborhood.
 *
 * @warning Values from 0 to 9 are not explicitly listed here, but can be used
 * normally with this enumeration type. For example, `constraint n = 3` is
 * correct.
 **/
typedef enum
{
  UNCONSTRAINED = -1,  /**< unconstrained square */
  MIN_CONSTRAINT = -1, /**< minimum square constraint */
  MAX_CONSTRAINT = 9   /**< maximum square constraint */
} constraint;

/** square status
 * @details These constants are used to indicate the status of a square in the
 * grid, that can be either ERROR, SATISFIED or UNSATISFIED  depending on the
 * colors of its 3x3 neighborhood (including the square itself). More precisely,
 * the ERROR status indicates that the number of black (or white) squares in the
 * neighborhood is too high, so that the numerical square constraint cannot be
 * met. The SATISFIED status indicates that the number of black squares in the
 * neighborhood is equal to the numerical square constraint, and that the other
 * squares are white, i.e. there are no more EMPTY squares in the 3x3
 * neighborhood. Finally, the status UNSATISFIED indicates that the numerical
 * constraint has not yet been satisfied, but may still be. In this case, there
 * are still EMPTY squares in the neighborhood, but there are no obvious errors.
 *
 * @warning All theses statuses are mutually exclusive, i.e. a square cannot be
 * both ERROR and UNSATISFIED.
 **/
typedef enum
{
  ERROR,       /**< error status */
  UNSATISFIED, /**< unsatisfied status */
  SATISFIED    /**< satisfied status */
} status;

/**
 * @brief All possible directions in the game grid to explore the neighbourhood
 * of a given square.
 **/
typedef enum
{
  HERE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT
} direction;

/**
 * @brief The structure pointer that stores the game state.
 **/
typedef struct game_s *game;

/**
 * @brief The structure constant pointer that stores the game state.
 * @details That means that it is not possible to modify the game using this
 * pointer.
 **/
typedef const struct game_s *cgame;

/**
 * @brief Creates a new game with default size and initializes it.
 *
 * @param constraints an array describing the initial constraint of each square
 *using row-major storage (of size DEFAULT_SIZE squared)
 * @param colors an array describing the initial color of each square using
 * row-major storage  (of size DEFAULT_SIZE squared) or NULL to set all colors
 * as EMPTY
 * @pre @p constraints must be an initialized array of default size squared
 * @pre @p colors must be an initialized array of default size squared or NULL
 * @return the created game
 **/
game game_new(constraint *constraints, color *colors);

/**
 * @brief Creates a new empty game with defaut size.
 *
 * @details All squares are initialized with unconstrained squares, with empty
 * color.
 * @return the created game
 **/
game game_new_empty(void);

/**
 * @brief Duplicates a game.
 *
 * @param g the game to copy
 * @return the copy of the game
 * @pre @p g must be a valid pointer toward a game structure.
 **/
game game_copy(cgame g);

/**
 * @brief Tests if two games are equal.
 *
 * @param g1 the first game
 * @param g2 the second game
 * @return true if the two games are equal, false otherwise
 * @pre @p g1 must be a valid pointer toward a game structure.
 * @pre @p g2 must be a valid pointer toward a game structure.
 **/
bool game_equal(cgame g1, cgame g2);

/**
 * @brief Deletes the game and frees the allocated memory.
 * @param g the game to delete
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_delete(game g);

/**
 * @brief Sets the constraint of a given square.
 *
 * @details This function is useful for initializing the squares of an empty
 * game.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param n the square constraint (or -1 if unconstrained square)
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre @p n >= MIN_CONSTRAINT and @p n <= MAX_CONSTRAINT
 **/
void game_set_constraint(game g, uint i, uint j, constraint n);

/**
 * @brief Sets the color of a given square.
 *
 * @details This function is useful for initializing the squares of an empty
 * game.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param c the square color
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre @p c must be a valid color (BLACK, WHITE or EMPTY)
 **/
void game_set_color(game g, uint i, uint j, color c);

/**
 * @brief Gets the constraint of a given square.
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the square constraint or -1 if unconstrained square
 **/
constraint game_get_constraint(cgame g, uint i, uint j);

/**
 * @brief Gets the color of a given square.
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the square color
 **/
color game_get_color(cgame g, uint i, uint j);

/**
 * @brief Gets the coordinate of the next square in a given direction.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param dir the direction
 * @param[out] pi_next the row index of the next square (output)
 * @param[out] pj_next the column index of the next square (output)
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return true if the next square is inside the grid, false otherwise
 */
bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next,
                          uint *pj_next);

/**
 * @brief Gets the square status.
 *
 * @param g the game
 * @param i the row index
 * @param j the column index
 * @return the square status, either ERROR, UNSATISFIED or SATISFIED.
 */
status game_get_status(cgame g, uint i, uint j);

/**
 * @brief Returns the number of neighbors of the same color.
 *
 * @details This function counts the number of squares of color @p c in the 3x3
 * neighborhood of the square at coordinate (i, j), including the square itself
 * in this count.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param c the choosen color
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the number of neighbors of the same color
 */
int game_nb_neighbors(cgame g, uint i, uint j, color c);

/**
 * @brief Plays a move in a given square.
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @param c the color to play
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre @p c must be a valid color (BLACK, WHITE or EMPTY)
 **/
void game_play_move(game g, uint i, uint j, color c);

/**
 * @brief Checks if the game is won.
 *
 * @param g the game
 * @details This function checks that all the game rules are satisfied, i.e. all
 * squares have color WHITE or BLACK with status SATISFIED.
 * @return true if the game is won, false otherwise
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 **/
bool game_won(cgame g);

/**
 * @brief Restarts a game.
 *
 * @details All the game is reset to its initial state. In particular, all the
 * colors are removed.
 * @param g the game
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_restart(game g);

#endif // __GAME_H__
