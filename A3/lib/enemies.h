#ifndef ENEMIES_H
#define ENEMIES_H 1

#include "camera.h"
#include "duck.h"
#include "entity.h"
#include "sprites.h"

#define ENEMY_HEALTH 2
#define ENEMY_ATTACK_RATE (int)(24 * (FPS / 30.0))
#define ENEMY_ATTACK_RADIUS (RENDER_WIDTH / 2.0) - (SPRITE_MAGE_IDLE_W * 2.0)

#define BOSS_HEALTH 10
#define BOSS_ATTACK_RATE (int)(12 * (FPS / 30.0))

void enemies_init ();

void enemies_update (ENTITY *duck);

void enemies_draw (CAMERA *cam, SPRITES *sprites);

void enemies_bullets_hit ();

#endif /* enemies.h */
