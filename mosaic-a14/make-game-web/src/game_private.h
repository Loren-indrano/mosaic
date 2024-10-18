/**
 * @file game_private.h
 * @brief Private Game Functions.
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#ifndef __GAME_PRIVATE_H__
#define __GAME_PRIVATE_H__

#include <stdbool.h>

#include "game.h"
#include "game_struct.h"
#include "queue.h"

/* ************************************************************************** */
/*                             Water Mark                                     */
/* ************************************************************************** */

extern const int __PT2_UBX_TEACHER__;

/* ************************************************************************** */
/*                             DATA TYPES                                     */
/* ************************************************************************** */

/**
 * @brief Move structure.
 * @details This structure is used to save the game history.
 */
struct move_s {
  uint i, j;
  color oldc, newc;
};

typedef struct move_s move;

/* ************************************************************************** */
/*                                MACRO                                       */
/* ************************************************************************** */

#define MAX(x, y) ((x > (y)) ? (x) : (y))

/* ************************************************************************** */
/*                             STACK ROUTINES                                 */
/* ************************************************************************** */

/** push a move in the stack */
void _stack_push_move(queue* q, move m);

/** pop a move from the stack */
move _stack_pop_move(queue* q);

/** test if the stack is empty */
bool _stack_is_empty(queue* q);

/** clear all the stack */
void _stack_clear(queue* q);

/* ************************************************************************** */
/*                                MISC                                        */
/* ************************************************************************** */

/** convert a char into constraint */
constraint _str2constraint(char c);

/** convert a char into color */
color _str2color(char c);

/** convert a constraint into its char representation */
char _constraint2str(constraint n);

/** convert a color into its char representation */
char _color2str(color c);

/** convert a square into its string representation
 * @details a single utf8 wide char represented by a string
 */
char* _square2str(constraint n, color c);

#endif  // __GAME_PRIVATE_H__
