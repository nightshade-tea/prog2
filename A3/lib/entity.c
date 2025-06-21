#include <math.h>
#include <stdlib.h>

#include "entity.h"
#include "sprites.h"

ENTITY *
ent_create (float px, float py, float szx, float szy, float vx, float vy,
            float ax, float ay, SPRITE_ID sid, unsigned char flip)
{
  ENTITY *ent;

  if (!(ent = malloc (sizeof *ent)))
    return NULL;

  ent->p.x = px;
  ent->p.y = py;

  ent->sz.x = szx;
  ent->sz.y = szy;

  ent->q.x = ent->p.x + ent->sz.x;
  ent->q.y = ent->p.y + ent->sz.y;

  ent->v.x = vx;
  ent->v.y = vy;

  ent->a.x = ax;
  ent->a.y = ay;

  ent->sid = sid;
  ent->flip = flip;

  return ent;
}

void
ent_destroy (ENTITY *ent)
{
  free (ent);
}

void
ent_update_velocity (ENTITY *ent)
{
  ent->v.x += ent->a.x;
  ent->v.y += ent->a.y;
}

void
ent_update_position (ENTITY *ent)
{
  ent->p.x += ent->v.x;
  ent->p.y += ent->v.y;
  ent->q.x += ent->v.x;
  ent->q.y += ent->v.y;
}

void
ent_keep_inside_bounds (ENTITY *ent, CAMERA *cam)
{
  if (ent->p.x - cam->offx < 0)
    {
      ent->p.x = cam->offx;
      ent->q.x = ent->p.x + ent->sz.x;
      ent->v.x = 0;
    }

  else if (ent->q.x - cam->offx > RENDER_WIDTH)
    {
      ent->q.x = RENDER_WIDTH + cam->offx;
      ent->p.x = ent->q.x - ent->sz.x;
      ent->v.x = 0;
    }

  if (ent->p.y < 0)
    {
      ent->p.y = 0;
      ent->q.y = ent->p.y + ent->sz.y;
      ent->v.y = 0;
    }

  else if (ent->q.y > RENDER_HEIGHT)
    {
      ent->q.y = RENDER_HEIGHT;
      ent->p.y = ent->q.y - ent->sz.y;
      ent->v.y = 0;
    }
}

static void
ent_move (ENTITY *ent, float x, float y)
{
  ent->p.x += x;
  ent->q.x += x;
  ent->p.y += y;
  ent->q.y += y;
}

unsigned char
ent_collides (ENTITY *ent, OBJECT *obj, float delta)
{
  if (ent->p.x + delta > obj->q.x)
    return 0;

  if (ent->q.x - delta < obj->p.x)
    return 0;

  if (ent->p.y + delta > obj->q.y)
    return 0;

  if (ent->q.y - delta < obj->p.y)
    return 0;

  return 1;
}

unsigned char
ent_on_top_of (ENTITY *ent, OBJECT *obj)
{
  if (ent->q.y < obj->p.y)
    return 0;

  if (ent->q.x < obj->p.x || ent->p.x > obj->q.x)
    return 0;

  return 1;
}

void
ent_collide (ENTITY *ent, OBJECT *obj)
{
  PAIR entc, objc; // ent,obj centers
  PAIR d;          // entc - objc
  PAIR overlap;

  if (!ent_collides (ent, obj, ENT_COLLISION_DELTA))
    return;

  entc.x = ent->p.x + (ent->sz.x / 2.0);
  entc.y = ent->p.y + (ent->sz.y / 2.0);

  objc.x = obj->p.x + (obj->sz.x / 2.0);
  objc.y = obj->p.y + (obj->sz.y / 2.0);

  d.x = entc.x - objc.x;
  d.y = entc.y - objc.y;

  overlap.x = ((ent->sz.x + obj->sz.x) / 2.0) - fabs (d.x);
  overlap.y = ((ent->sz.y + obj->sz.y) / 2.0) - fabs (d.y);

  // resolve along the smallest overlap axis
  if (overlap.x < overlap.y)
    {
      if (d.x > 0)
        ent_move (ent, overlap.x, 0);

      else
        ent_move (ent, -overlap.x, 0);

      ent->v.x = 0;
    }

  else
    {
      if (d.y > 0)
        ent_move (ent, 0, overlap.y);

      else
        {
          ent_move (ent, 0, -overlap.y);
          ent->v.y = 0;
        }
    }
}
