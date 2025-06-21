#include <math.h>
#include <stdlib.h>

#include "camera.h"
#include "duck.h"
#include "game.h"
#include "keyboard.h"
#include "object.h"
#include "sprites.h"

ENTITY *
duck_create ()
{
  return ent_create (DUCK_PX, DUCK_PY, DUCK_SZX, DUCK_SZY, 0, 0, 0, GRAV,
                     SPRITE_DUCK_IDLE, 0);
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

void
duck_update (ENTITY *duck, KEYBOARD key[ALLEGRO_KEY_MAX], SPRITES *sprites,
             CAMERA *cam)
{
  float ax;
  float vx;
  float threshold;

  // inertia
  if (fabs (duck->v.x) <= DUCK_INERTIA)
    duck->a.x = duck->v.x = 0;

  else if (duck->v.x > 0)
    duck->a.x = -1 * DUCK_INERTIA;

  else
    duck->a.x = DUCK_INERTIA;

  // gliding
  if (duck->sid == SPRITE_DUCK_FALL && (duck->v.y >= DUCK_GLIDESPD)
      && (key[ALLEGRO_KEY_SPACE] & KEY_DOWN))
    {
      duck->a.y = 0;
      duck->v.y = DUCK_GLIDESPD;
    }

  else
    duck->a.y = GRAV;

  // handle keypresses
  if (key[ALLEGRO_KEY_SPACE] & KEY_SEEN)
    {
      duck->v.y = -1 * DUCK_JMPSPD * (duck->q.y / RENDER_HEIGHT);
      sprites_reset_state (sprites, SPRITE_DUCK_JUMP);
    }

  if (key[ALLEGRO_KEY_A] || key[ALLEGRO_KEY_D])
    {
      if (key[ALLEGRO_KEY_S] && duck->v.y == 0)
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
        threshold = DUCK_INERTIA;

      if (fabs (duck->v.x - vx) <= threshold)
        {
          duck->a.x = 0;
          duck->v.x = vx;
        }

      else if (duck->v.x > vx)
        duck->a.x = -1 * DUCK_INERTIA;

      else
        duck->a.x = ax;
    }

  if (key[ALLEGRO_KEY_A])
    {
      ax *= -1;
      vx *= -1;

      if (duck->v.x < vx)
        threshold = DUCK_INERTIA;
      else
        threshold = -ax;

      if (fabs (duck->v.x - vx) <= threshold)
        {
          duck->a.x = 0;
          duck->v.x = vx;
        }

      else if (duck->v.x < vx)
        duck->a.x = DUCK_INERTIA;

      else
        duck->a.x = ax;
    }

  // update sprite id
  if (!key[ALLEGRO_KEY_A] && !key[ALLEGRO_KEY_D])
    {
      if (key[ALLEGRO_KEY_S])
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

  if (duck->p.x + duck->v.x - cam->offx < 0)
    {
      duck->sid = SPRITE_DUCK_WALL_HIT;
      update_geometry (duck);
      duck->p.x = cam->offx;
      duck->q.x = duck->p.x + duck->sz.x;
    }

  else if (duck->q.x + duck->v.x - cam->offx > RENDER_WIDTH)
    {
      duck->sid = SPRITE_DUCK_WALL_HIT;
      update_geometry (duck);
      duck->p.x = RENDER_WIDTH + cam->offx;
      duck->q.x = duck->p.x + duck->sz.x;
    }

  if (duck->v.y < 0)
    duck->sid = SPRITE_DUCK_JUMP;

  else if (duck->q.y + duck->v.y < RENDER_HEIGHT)
    duck->sid = SPRITE_DUCK_FALL;

  update_geometry (duck);
  ent_update_position (duck);
  ent_keep_inside_bounds (duck, cam);
}
