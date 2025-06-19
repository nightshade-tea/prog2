#include <math.h>
#include <stdlib.h>

#include "def.h"
#include "duck.h"
#include "keyboard.h"
#include "object.h"
#include "sprites.h"

ENTITY *
duck_create ()
{
  return ent_create (DUCK_PX, DUCK_PY, DUCK_SZX, DUCK_SZY, 0, 0, 0, GRAV,
                     SPRITE_DUCK_IDLE, 0);
}

void
duck_update (ENTITY *duck, KEYBOARD key[ALLEGRO_KEY_MAX], SPRITES *sprites)
{
  float ax = 0;
  float vx = 0;
  float threshold;

  // inertia
  if (fabs (duck->v.x) <= DUCK_INERTIA)
    duck->a.x = duck->v.x = 0;

  else if (duck->v.x > 0)
    duck->a.x = -1 * DUCK_INERTIA;

  else
    duck->a.x = DUCK_INERTIA;

  // gliding
  if ((duck->v.y >= DUCK_GLIDESPD) && (key[ALLEGRO_KEY_SPACE] & KEY_DOWN))
    {
      duck->a.y = 0;
      duck->v.y = DUCK_GLIDESPD;
    }

  else
    duck->a.y = GRAV;

  // handle keypresses
  if (key[ALLEGRO_KEY_SPACE] & KEY_SEEN)
    {
      duck->v.y = -1 * DUCK_JMPSPD;
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

  ent_update_velocity (duck);
  ent_update_position (duck);
  ent_keep_inside_bounds (duck);

  // update flip flag
  if (duck->v.x > 0)
    duck->flip = 0;

  else if (duck->v.x < 0)
    duck->flip = 1;

  // update sprite id
  if (duck->v.x == 0 || (!key[ALLEGRO_KEY_A] && !key[ALLEGRO_KEY_D]))
    {
      if (key[ALLEGRO_KEY_S])
        duck->sid = SPRITE_DUCK_CROUCH;
      else
        duck->sid = SPRITE_DUCK_IDLE;
    }

  if (duck->v.y < 0)
    duck->sid = SPRITE_DUCK_JUMP;

  else if (duck->v.y > 0)
    duck->sid = SPRITE_DUCK_FALL;
}
