#ifndef DUCK_H
#define DUCK_H 1

#include "entity.h"
#include "keyboard.h"
#include "sprites.h"

#define DUCK_SZX 23
#define DUCK_SZY 20

#define DUCK_PX 0
#define DUCK_PY RENDER_HEIGHT - DUCK_SZY

#define DUCK_CRAWL_SZX 34
#define DUCK_CRAWL_SZY 9

#define DUCK_ROLLSPD 17.25 / (FPS / 30)
#define DUCK_ROLLACC DUCK_ROLLSPD / 30

#define DUCK_WALKSPD DUCK_ROLLSPD / 2
#define DUCK_WALKACC DUCK_WALKSPD / 15

#define DUCK_CRAWLSPD DUCK_WALKSPD / 2
#define DUCK_CRAWLACC DUCK_CRAWLSPD / 5

#define DUCK_INERTIA DUCK_WALKSPD / 20

#define DUCK_JMPSPD 25.0 / (FPS / 30)
#define DUCK_GLIDESPD GRAV * 2

#define DUCK_DEFAULT_SPRITE SPRITE_DUCK_IDLE

ENTITY *duck_create ();

void duck_update (ENTITY *duck, KEYBOARD key[ALLEGRO_KEY_MAX],
                  SPRITES *sprites);

#endif /* duck.h */
