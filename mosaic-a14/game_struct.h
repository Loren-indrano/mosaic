#include <stdbool.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

typedef struct {
  color col;
  constraint cons;
} Case;

// stocke un movement et ses coordonées
typedef struct move_s {
  uint i;
  uint j;
  color old_col;
  color new_col;
} move;

typedef struct game_s {
  bool wrapping;
  neighbourhood neigh;
  uint columns;
  uint rows;
  Case **grille;
  queue *history;
  queue *old_moves;

} game_S;

struct queue_s {
  struct element_s *head;
  struct element_s *tail;
  unsigned int length;
};

/* *********************************************************** */

struct element_s {
  void *data;
  struct element_s *next;
  struct element_s *prev;
};

/* *********************************************************** */

typedef struct element_s element_t;

/* *********************************************************** */

void *safeMalloc(int size);

void game_set_status(game g, uint i, uint j);