#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "bullets.h"
#include "entity.h"
#include "game.h"
#include "object.h"
#include "platforms.h"
#include "sprites.h"
#include "tiles.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern OBJECT platforms[];
extern const size_t platforms_num;

ENTITY *bullets[BULLETS_MAX];
static size_t next;

void
bullets_init ()
{
  memset (bullets, 0, sizeof bullets);
  next = 0;
}

void
bullets_destroy ()
{
  for (size_t i = 0; i < BULLETS_MAX; i++)
    if (bullets[i])
      ent_destroy (bullets[i]);
}

static void
bullet_destroy (ENTITY **b)
{
  if (!b || !(*b))
    return;

  ent_destroy (*b);
  *b = NULL;
}

void
bullet_create (float px, float py, SPRITE_ID sid, unsigned char flip,
               float angle)
{
  float szx, szy;
  float vx, vy;
  float ax, ay;
  float theta;

  switch (sid)
    {
    case SPRITE_DUCK_QUACK:
      szx = SPRITE_DUCK_QUACK_W;
      szy = SPRITE_DUCK_QUACK_H;
      break;

    case SPRITE_MAGE_BULLET:
      szx = SPRITE_MAGE_BULLET_W;
      szy = SPRITE_MAGE_BULLET_H;
      break;

    default:
      return;
    }

  theta = angle * (M_PI / 2.0);

  vx = cosf (theta) * BULLET_SPEED;
  vy = -sinf (theta) * BULLET_SPEED;

  ax = cosf (theta) * BULLET_ACCEL;
  ay = -sinf (theta) * BULLET_ACCEL;

  if (flip)
    {
      vx *= -1;
      ax *= -1;
    }

  bullet_destroy (&bullets[next]);

  bullets[next] = ent_create (px, py, szx, szy, vx, vy, ax, ay, sid,
                              TILE_NODRAW, flip, BULLET_DAMAGE);

  next = (next + 1) % BULLETS_MAX;
}

void
bullets_update (CAMERA *cam)
{
  bool collision;

  for (size_t i = 0; i < BULLETS_MAX; i++)
    if (bullets[i])
      {
        ent_update_velocity (bullets[i]);
        ent_update_position (bullets[i]);

        if ((bullets[i]->q.x - cam->offx) < 0.0
            || (bullets[i]->p.x - cam->offx) > RENDER_WIDTH)
          {
            bullet_destroy (&bullets[i]);
            continue;
          }

        if ((bullets[i]->q.y) < 0.0 || (bullets[i]->p.y) > RENDER_HEIGHT)
          {
            bullet_destroy (&bullets[i]);
            continue;
          }

        collision = false;

        for (size_t j = 0; j < platforms_num; j++)
          if (ent_collides (bullets[i], &platforms[j], ENT_COLLISION_DELTA))
            {
              collision = true;
              break;
            }

        if (collision)
          {
            bullet_destroy (&bullets[i]);
            continue;
          }
      }
}

void
bullets_draw (CAMERA *cam, SPRITES *sprites)
{
  for (size_t i = 0; i < BULLETS_MAX; i++)
    if (bullets[i])
      obj_draw ((OBJECT *)bullets[i], cam, sprites);
}

void
bullets_hit (ENTITY *ent)
{
  for (size_t i = 0; i < BULLETS_MAX; i++)
    if (bullets[i]
        && ent_collides (ent, (OBJECT *)bullets[i], ENT_COLLISION_DELTA))
      {
        switch (ent->sid)
        {
          case SPRITE_DUCK_CRAWL:
          case SPRITE_DUCK_CROUCH:

            bullets[i]->v.x *= -1.0;
            bullets[i]->v.y *= -1.0;
            bullets[i]->a.x *= -1.0;
            bullets[i]->a.y *= -1.0;

            if (bullets[i]->v.x > 0)
              {
                bullets[i]->p.x += ent->sz.x * 0.5;
                bullets[i]->q.x += ent->sz.x * 0.5;
              }

            else
              {
                bullets[i]->p.x -= ent->sz.x * 0.5;
                bullets[i]->q.x -= ent->sz.x * 0.5;
              }

            bullets[i]->flip = !bullets[i]->flip;

            break;

          default:
            if (ent->health)
              ent->health--;

            bullet_destroy (&bullets[i]);

            break;
        }
      }
}
