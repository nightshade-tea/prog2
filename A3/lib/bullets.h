#ifndef BULLETS_H
#define BULLETS_H 1

#include "camera.h"
#include "entity.h"
#include "sprites.h"

#define BULLETS_MAX 1024

#define BULLET_SPEED 5.0
#define BULLET_ACCEL BULLET_SPEED / 10.0
#define BULLET_DAMAGE 1

void bullets_init ();

void bullets_destroy ();

void bullet_create (float px, float py, SPRITE_ID sid, unsigned char flip,
                    float angle);

void bullets_update (CAMERA *cam);

void bullets_draw (CAMERA *cam, SPRITES *sprites);

void bullets_hit (ENTITY *ent);

#endif /* bullets.h */
