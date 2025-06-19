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
ent_keep_inside_bounds (ENTITY *ent)
{
  if (ent->p.x < 0.5)
    {
      ent->p.x = 0.5;
      ent->q.x = ent->p.x + ent->sz.x;
      ent->v.x = 0;
    }

  else if (ent->q.x > RENDER_WIDTH - 0.5)
    {
      ent->q.x = RENDER_WIDTH - 0.5;
      ent->p.x = ent->q.x - ent->sz.x;
      ent->v.x = 0;
    }

  if (ent->p.y < 0.5)
    {
      ent->p.y = 0.5;
      ent->q.y = ent->p.y + ent->sz.y;
    }

  else if (ent->q.y > RENDER_HEIGHT - 0.5)
    {
      ent->q.y = RENDER_HEIGHT - 0.5;
      ent->p.y = ent->q.y - ent->sz.y;
      ent->v.y = 0;
    }
}
