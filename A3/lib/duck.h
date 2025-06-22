#ifndef DUCK_H
#define DUCK_H 1

#include "camera.h"
#include "entity.h"
#include "keyboard.h"
#include "sprites.h"

#define DUCK_DEFAULT_SPRITE SPRITE_DUCK_IDLE

#define DUCK_DEFAULT_SZX SPRITE_DUCK_IDLE_W
#define DUCK_DEFAULT_SZY SPRITE_DUCK_IDLE_H

#define DUCK_DEFAULT_PX RENDER_WIDTH / 3.0
#define DUCK_DEFAULT_PY (RENDER_HEIGHT - DUCK_DEFAULT_SZY) / 2.0

#define DUCK_ROLLSPD 17.25 / (FPS / 30.0)
#define DUCK_ROLLACC DUCK_ROLLSPD / 30.0

#define DUCK_WALKSPD DUCK_ROLLSPD / 2.0
#define DUCK_WALKACC DUCK_WALKSPD / 15.0

#define DUCK_CRAWLSPD DUCK_WALKSPD / 2.0
#define DUCK_CRAWLACC DUCK_CRAWLSPD / 5.0

#define DUCK_INERTIA DUCK_WALKSPD / 20.0
#define DUCK_AIR_RESISTANCE DUCK_INERTIA / 6.0

#define DUCK_JMPSPD 25.0 / (FPS / 30.0)
#define DUCK_GLIDESPD GRAV * 2.0

#define DUCK_HEALTH 1

ENTITY *duck_create ();

void duck_update (ENTITY *duck, KEYBOARD key[ALLEGRO_KEY_MAX],
                  SPRITES *sprites, CAMERA *cam);

#endif /* duck.h */
