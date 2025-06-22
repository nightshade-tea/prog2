#ifndef OBJECT_H
#define OBJECT_H 1

#include "camera.h"
#include "sprites.h"
#include "tiles.h"

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
  PAIR p, q;          // upper left and bottom right points
  PAIR sz;            // size vector (q.x - p.x, q.y - p.y)
  SPRITE_ID sid;      // sprite for the object
  TILE_TYPE ttype;    // tile for the object
  unsigned char flip; // 1 = flip horizontally when drawing
} OBJECT;

OBJECT *obj_create (float px, float py, float szx, float szy, SPRITE_ID sid,
                    TILE_TYPE ttype, unsigned char flip);

void obj_destroy (OBJECT *obj);

void obj_draw (OBJECT *obj, CAMERA *cam, SPRITES *sprites);

#endif /* object.h */
