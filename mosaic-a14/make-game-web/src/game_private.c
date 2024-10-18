/**
 * @file game_private.c
 * @copyright University of Bordeaux. All rights reserved, 2023.
 **/

#include "game_private.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

/* ************************************************************************** */
/*                             STACK ROUTINES                                 */
/* ************************************************************************** */

void _stack_push_move(queue* q, move m)
{
  assert(q);
  move* pm = malloc(sizeof(move));
  assert(pm);
  *pm = m;
  queue_push_head(q, pm);
}

/* ************************************************************************** */

move _stack_pop_move(queue* q)
{
  assert(q);
  move* pm = queue_pop_head(q);
  assert(pm);
  move m = *pm;
  free(pm);
  return m;
}

/* ************************************************************************** */

bool _stack_is_empty(queue* q)
{
  assert(q);
  return queue_is_empty(q);
}

/* ************************************************************************** */

void _stack_clear(queue* q)
{
  assert(q);
  queue_clear_full(q, free);
  assert(queue_is_empty(q));
}

/* ************************************************************************** */
/*                                  AUXILIARY                                 */
/* ************************************************************************** */

constraint _str2constraint(char c)
{
  if (c == '-') return -1;
  if (c >= '0' && c <= '9') return c - '0';
  return -2; /* invalid */
}

/* ************************************************************************** */

color _str2color(char c)
{
  if (c == 'e') return EMPTY;
  if (c == 'w') return WHITE;
  if (c == 'b') return BLACK;
  return -1; /* invalid */
}

/* ************************************************************************** */

char _color2str(color c)
{
  char str_col[3] = {'e', 'w', 'b'};
  return str_col[c];
}

/* ************************************************************************** */

char _constraint2str(constraint n)
{
  if (n == -1) return '-';
  if (n >= 0 && n <= 9) return '0' + n;
  return -1; /* invalid */
}

/* ************************************************************************** */

char* col2str[3] = {" ", "□", "■"};
// char *col2str[3] = {" ", "Ⓤ", "🅤"};
char* num2str[3][10] = {
    {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"},     // empty
    {"🄋", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈"},  // white
    {"⓿", "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾"},     // black
};

char* _square2str(constraint n, color c)
{
  if (n == UNCONSTRAINED)
    return col2str[c];
  else
    return num2str[c][n];
}

/* ************************************************************************** */
/*                             WATERMARK                                      */
/* ************************************************************************** */

void __attribute__((constructor)) watermark()
{
  fprintf(stderr, "Copyright: Mosaic Game by University of Bordeaux, 2023.\n");
  system("date >> watermark");
}

/* ************************************************************************** */