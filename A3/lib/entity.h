#ifndef ENTITY_H
#define ENTITY_H 1

#include "object.h"
#include "sprites.h"

/* an entity is an object with movement vectors. */
typedef struct ENTITY
{
  PAIR p, q; // object properties
  PAIR sz;   // ...
  SPRITE_ID sid;
  unsigned char flip;

  PAIR v; // velocity vector
  PAIR a; // acceleration vector
} ENTITY;

ENTITY *ent_create (float px, float py, float szx, float szy, float vx,
                    float vy, float ax, float ay, SPRITE_ID sid,
                    unsigned char flip);

void ent_destroy (ENTITY *ent);

/* v += a */
void ent_update_velocity (ENTITY *ent);

/* p,q += v */
void ent_update_position (ENTITY *ent);

/* updates object points to keep it inside the screen limits.
 * if a collision occurs, the velocity vector is updated accordingly. */
void ent_keep_inside_bounds (ENTITY *ent);

#endif /* entity.h */
