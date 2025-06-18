#include <stdbool.h>
#include <stdlib.h>

#include "def.h"
#include "object.h"

OBJECT *
obj_create (float px, float py, float szx, float szy)
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

  return obj;
}

void
obj_destroy (OBJECT *obj)
{
  free (obj);
}

bool
obj_collides (OBJECT *a, OBJECT *b)
{
  if (a->p.x > b->q.x)
    return false;

  if (a->q.x < b->p.x)
    return false;

  if (a->p.y > b->q.y)
    return false;

  if (a->q.y < b->p.y)
    return false;

  return true;
}

void
obj_keep_inside_bounds (OBJECT *obj)
{
  if (obj->p.x < 0.5)
    {
      obj->p.x = 0.5;
      obj->q.x = obj->p.x + obj->sz.x;
    }

  else if (obj->q.x > RENDER_WIDTH - 0.5)
    {
      obj->q.x = RENDER_WIDTH - 0.5;
      obj->p.x = obj->q.x - obj->sz.x;
    }

  if (obj->p.y < 0.5)
    {
      obj->p.y = 0.5;
      obj->q.y = obj->p.y + obj->sz.y;
    }

  else if (obj->q.y > RENDER_HEIGHT - 0.5)
    {
      obj->q.y = RENDER_HEIGHT - 0.5;
      obj->p.y = obj->q.y - obj->sz.y;
    }
}
