#include <math.h>
#include <stdlib.h>

#include "def.h"
#include "duck.h"
#include "keyboard.h"

ENTITY *
duck_create ()
{
  return ent_create (DUCK_PX, DUCK_PY, DUCK_SZX, DUCK_SZY, 0, 0, 0, GRAV);
}

void
duck_update_position (ENTITY *duck, KEYBOARD key[ALLEGRO_KEY_MAX])
{
  // inertia
  if (fabs (duck->v.x) <= 2 * DUCK_INERTIA)
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

  ent_update_velocity (duck);

  // handle keypresses
  if (key[ALLEGRO_KEY_SPACE] & KEY_SEEN)
    duck->v.y = -1 * DUCK_JMPSPD;

  if (key[ALLEGRO_KEY_A])
    duck->v.x = -1 * DUCK_WALKSPD;

  if (key[ALLEGRO_KEY_D])
    duck->v.x = DUCK_WALKSPD;

  ent_update_position (duck);
}
