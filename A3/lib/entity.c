#include <stdlib.h>

#include "entity.h"

ENTITY *
ent_create (float px, float py, float szx, float szy, float vx, float vy,
            float ax, float ay)
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
