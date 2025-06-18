#ifndef ENTITY_H
#define ENTITY_H 1

#include "object.h"

/* an entity is an object with movement vectors. */
typedef struct ENTITY
{
  PAIR p, q; // object properties
  PAIR sz;   // ...
  PAIR v;    // velocity vector
  PAIR a;    // acceleration vector
} ENTITY;

ENTITY *ent_create (float px, float py, float szx, float szy, float vx,
                    float vy, float ax, float ay);

void ent_destroy (ENTITY *ent);

/* v += a */
void ent_update_velocity (ENTITY *ent);

/* p,q += v */
void ent_update_position (ENTITY *ent);

#endif /* entity.h */
