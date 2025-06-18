#ifndef OBJECT_H
#define OBJECT_H 1

#include <stdbool.h>

typedef struct PAIR
{
  float x, y;
} PAIR;

/* p ----------+
 * | \         |
 * |   \       |
 * |    sz     |
 * |       \   |
 * |         \ |
 * +-----------q */
typedef struct OBJECT
{
  PAIR p, q; // upper left and bottom right points
  PAIR sz;   // size vector (q.x - p.x, q.y - p.y)
} OBJECT;

OBJECT *obj_create (float px, float py, float szx, float szy);

void obj_destroy (OBJECT *obj);

bool obj_collides (OBJECT *a, OBJECT *b);

/* updates object points to keep it inside the screen limits */
void obj_keep_inside_bounds (OBJECT *obj);

#endif /* object.h */
