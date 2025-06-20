#include <allegro5/allegro_primitives.h>

#include <stdbool.h>
#include <stdlib.h>

#include "camera.h"
#include "game.h"
#include "object.h"

OBJECT *
obj_create (float px, float py, float szx, float szy, SPRITE_ID sid,
            unsigned char flip)
{
  OBJECT *obj;

  if (!(obj = malloc (sizeof *obj)))
    return NULL;

  obj->p.x = px;
  obj->p.y = py;

  obj->sz.x = szx;
  obj->sz.y = szy;

  obj->q.x = obj->p.x + obj->sz.x;
  obj->q.y = obj->p.y + obj->sz.y;

  obj->sid = sid;
  obj->flip = flip;

  return obj;
}

void
obj_destroy (OBJECT *obj)
{
  free (obj);
}

unsigned char
obj_collides (OBJECT *a, OBJECT *b)
{
  if (a->p.x > b->q.x)
    return 0;

  if (a->q.x < b->p.x)
    return 0;

  if (a->p.y > b->q.y)
    return 0;

  if (a->q.y < b->p.y)
    return 0;

  return 1;
}

void
obj_draw (OBJECT *obj, CAMERA *cam, SPRITES *sprites)
{
#if DBG
  al_draw_rectangle (obj->p.x - cam->offx + 0.5, obj->p.y + 0.5,
                     obj->q.x - cam->offx - 0.5, obj->q.y - 0.5,
                     al_map_rgb (255, 0, 0), 1);
#endif

  if (obj->sid == SPRITE_NODRAW)
    return;

  al_draw_bitmap (sprites_get (sprites, obj->sid), obj->p.x - cam->offx,
                  obj->p.y, obj->flip);
}
