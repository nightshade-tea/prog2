#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "camera.h"
#include "duck.h"
#include "game.h"
#include "keyboard.h"
#include "object.h"
#include "platforms.h"
#include "sprites.h"
#include "tiles.h"

extern OBJECT platforms[];
extern const size_t platforms_num;

ENTITY *
duck_create ()
{
  return ent_create (DUCK_DEFAULT_PX, DUCK_DEFAULT_PY, DUCK_DEFAULT_SZX,
                     DUCK_DEFAULT_SZY, 0, 0, 0, GRAV, DUCK_DEFAULT_SPRITE,
                     TILE_NODRAW, 0);
}

#define handle_case(X)                                                        \
  do                                                                          \
    {                                                                         \
      duck->q.x += (SPRITE_DUCK_##X##_W - duck->sz.x) / 2.0;                  \
                                                                              \
      duck->sz.x = SPRITE_DUCK_##X##_W;                                       \
      duck->sz.y = SPRITE_DUCK_##X##_H;                                       \
                                                                              \
      duck->p.x = duck->q.x - duck->sz.x;                                     \
      duck->p.y = duck->q.y - duck->sz.y;                                     \
    }                                                                         \
  while (0)

static void
update_geometry (ENTITY *duck)
{
  switch (duck->sid)
    {
    case SPRITE_DUCK_IDLE:
      handle_case (IDLE);
      break;

    case SPRITE_DUCK_WALK:
      handle_case (WALK);
      break;

    case SPRITE_DUCK_ROLL:
      handle_case (ROLL);
      break;

    case SPRITE_DUCK_JUMP:
      handle_case (JUMP);
      break;

    case SPRITE_DUCK_FALL:
      handle_case (FALL);
      break;

    case SPRITE_DUCK_CROUCH:
    case SPRITE_DUCK_CRAWL:
      handle_case (CRAWL);
      break;

    case SPRITE_DUCK_DEATH:
      handle_case (DEATH);
      break;

    case SPRITE_DUCK_WALL_HIT:
      handle_case (WALL_HIT);
      break;

    default:
      break;
    }
}

#undef handle_case

static void
wall_hit (ENTITY *duck, OBJECT *obj, KEYBOARD key[ALLEGRO_KEY_MAX])
{
  PAIR duckc, objc;
  PAIR d;
  PAIR overlap;

  if (duck->v.x == 0)
    return;

  if (duck->v.x < 0 && !key[ALLEGRO_KEY_A])
    return;

  if (duck->v.x > 0 && !key[ALLEGRO_KEY_D])
    return;

  if (!ent_collides (duck, obj, 0))
    return;

  duckc.x = duck->p.x + (duck->sz.x / 2.0);
  duckc.y = duck->p.y + (duck->sz.y / 2.0);

  objc.x = obj->p.x + (obj->sz.x / 2.0);
  objc.y = obj->p.y + (obj->sz.y / 2.0);

  d.x = duckc.x - objc.x;
  d.y = duckc.y - objc.y;

  overlap.x = ((duck->sz.x + obj->sz.x) / 2.0) - fabs (d.x);
  overlap.y = ((duck->sz.y + obj->sz.y) / 2.0) - fabs (d.y);

  if (overlap.x < overlap.y && ((duck->v.x < 0) != (d.x < 0)))
    {
      duck->sid = SPRITE_DUCK_WALL_HIT;
      update_geometry (duck);

      if (d.x > 0)
        {
          duck->p.x = obj->q.x;
          duck->q.x = duck->p.x + duck->sz.x;
        }

      else
        {
          duck->q.x = obj->p.x;
          duck->p.x = duck->q.x - duck->sz.x;
        }
    }
}

static unsigned char
in_tunnel (ENTITY *duck, OBJECT *obj)
{
  float dy = SPRITE_DUCK_IDLE_H - duck->sz.y;

  if (duck->p.y - dy >= obj->q.y || duck->q.y <= obj->p.y)
    return 0;

  if (duck->q.x <= obj->p.x || duck->p.x >= obj->q.x)
    return 0;

  return 1;
}

void
duck_update (ENTITY *duck, KEYBOARD key[ALLEGRO_KEY_MAX], SPRITES *sprites,
             CAMERA *cam)
{
  float ax;
  float vx;
  float threshold;
  float inertia;
  bool on_ground;
  bool tunnel;

  // inertia
  if (duck->v.y == 0)
    inertia = DUCK_INERTIA;

  else
    inertia = DUCK_AIR_RESISTANCE;

  if (fabs (duck->v.x) <= inertia)
    duck->a.x = duck->v.x = 0;

  else if (duck->v.x > 0)
    duck->a.x = -1 * inertia;

  else
    duck->a.x = inertia;

  // gliding
  if (duck->sid == SPRITE_DUCK_FALL && (duck->v.y >= DUCK_GLIDESPD)
      && (key[ALLEGRO_KEY_SPACE] & KEY_DOWN))
    {
      duck->a.y = 0;
      duck->v.y = DUCK_GLIDESPD;
    }

  else
    duck->a.y = GRAV;

  tunnel = false;

  for (size_t i = 0; i < platforms_num; i++)
    if (in_tunnel (duck, &platforms[i]))
      {
        tunnel = true;
        break;
      }

  // handle keypresses
  if (key[ALLEGRO_KEY_SPACE] & KEY_SEEN)
    {
      duck->v.y = -1 * DUCK_JMPSPD * (duck->q.y / RENDER_HEIGHT);
      sprites_reset_state (sprites, SPRITE_DUCK_JUMP);
    }

  if (key[ALLEGRO_KEY_A] || key[ALLEGRO_KEY_D])
    {
      if ((tunnel || key[ALLEGRO_KEY_S]) && duck->v.y == 0)
        {
          ax = DUCK_CRAWLACC;
          vx = DUCK_CRAWLSPD;
          duck->sid = SPRITE_DUCK_CRAWL;
        }

      else if (key[ALLEGRO_KEY_LSHIFT] && duck->v.y == 0)
        {
          ax = DUCK_ROLLACC;
          vx = DUCK_ROLLSPD;
          duck->sid = SPRITE_DUCK_ROLL;
        }

      else
        {
          ax = DUCK_WALKACC;
          vx = DUCK_WALKSPD;
          duck->sid = SPRITE_DUCK_WALK;
        }
    }

  if (key[ALLEGRO_KEY_D])
    {
      if (duck->v.x < vx)
        threshold = ax;
      else
        threshold = inertia;

      if (fabs (duck->v.x - vx) <= threshold)
        {
          duck->a.x = 0;
          duck->v.x = vx;
        }

      else if (duck->v.x > vx)
        duck->a.x = -1 * inertia;

      else
        duck->a.x = ax;
    }

  if (key[ALLEGRO_KEY_A])
    {
      ax *= -1;
      vx *= -1;

      if (duck->v.x < vx)
        threshold = inertia;
      else
        threshold = -ax;

      if (fabs (duck->v.x - vx) <= threshold)
        {
          duck->a.x = 0;
          duck->v.x = vx;
        }

      else if (duck->v.x < vx)
        duck->a.x = inertia;

      else
        duck->a.x = ax;
    }

  // update sprite id
  if (!key[ALLEGRO_KEY_A] && !key[ALLEGRO_KEY_D])
    {
      if (tunnel || key[ALLEGRO_KEY_S])
        duck->sid = SPRITE_DUCK_CROUCH;
      else
        duck->sid = SPRITE_DUCK_IDLE;
    }

  ent_update_velocity (duck);

  // update flip flag
  if (duck->v.x > 0)
    duck->flip = 0;

  else if (duck->v.x < 0)
    duck->flip = 1;

  on_ground = false;

  if (duck->q.y >= RENDER_HEIGHT)
    on_ground = true;

  for (size_t i = 0; i < platforms_num; i++)
    if (ent_on_top_of (duck, &platforms[i]))
      {
        on_ground = true;
        break;
      }

  if (duck->p.x + duck->v.x - cam->offx < 0
      && !(duck->v.x < 0 && !key[ALLEGRO_KEY_A]))
    {
      duck->sid = SPRITE_DUCK_WALL_HIT;
      update_geometry (duck);
      duck->p.x = cam->offx;
      duck->q.x = duck->p.x + duck->sz.x;
    }

  else if (duck->q.x + duck->v.x - cam->offx > RENDER_WIDTH
           && !(duck->v.x > 0 && !key[ALLEGRO_KEY_D]))
    {
      duck->sid = SPRITE_DUCK_WALL_HIT;
      update_geometry (duck);
      duck->p.x = RENDER_WIDTH + cam->offx;
      duck->q.x = duck->p.x + duck->sz.x;
    }

  for (size_t i = 0; i < platforms_num; i++)
    wall_hit (duck, &platforms[i], key);

  if (duck->v.y < 0)
    duck->sid = SPRITE_DUCK_JUMP;

  else if (!on_ground)
    duck->sid = SPRITE_DUCK_FALL;

  update_geometry (duck);
  ent_update_position (duck);

  for (size_t i = 0; i < platforms_num; i++)
    ent_collide (duck, &platforms[i]);

  ent_keep_inside_bounds (duck, cam);
}
