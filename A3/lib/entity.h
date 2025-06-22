#ifndef ENTITY_H
#define ENTITY_H 1

#include "camera.h"
#include "object.h"
#include "sprites.h"
#include "tiles.h"

#define ENT_COLLISION_DELTA 1.0e-3

/* an entity is an object with movement vectors and health. */
typedef struct ENTITY
{
  PAIR p, q; // object properties
  PAIR sz;   // ...
  SPRITE_ID sid;
  TILE_TYPE ttype;
  unsigned char flip;

  PAIR v; // velocity vector
  PAIR a; // acceleration vector
  unsigned char health;
} ENTITY;

ENTITY *ent_create (float px, float py, float szx, float szy, float vx,
                    float vy, float ax, float ay, SPRITE_ID sid,
                    TILE_TYPE ttype, unsigned char flip, unsigned char health);

void ent_destroy (ENTITY *ent);

/* v += a */
void ent_update_velocity (ENTITY *ent);

/* p,q += v */
void ent_update_position (ENTITY *ent);

/* updates object points to keep it inside the screen limits.
 * if a collision occurs, the velocity vector is updated accordingly. */
void ent_keep_inside_bounds (ENTITY *ent, CAMERA *cam);

unsigned char ent_collides (ENTITY *ent, OBJECT *obj, float delta);

unsigned char ent_on_top_of (ENTITY *ent, OBJECT *obj);

void ent_collide (ENTITY *ent, OBJECT *obj);

#endif /* entity.h */
