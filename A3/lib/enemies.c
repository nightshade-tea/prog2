#include <math.h>
#include <string.h>

#include "bullets.h"
#include "camera.h"
#include "enemies.h"
#include "entity.h"
#include "object.h"
#include "sprites.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern unsigned char end;

#define ENEMY(qx, qy, hp)                                                     \
  (ENTITY)                                                                    \
  {                                                                           \
    .p = { (qx) - SPRITE_MAGE_IDLE_W, (qy) - SPRITE_MAGE_IDLE_H },            \
    .q = { (qx), (qy) }, .sz = { SPRITE_MAGE_IDLE_W, SPRITE_MAGE_IDLE_H },    \
    .sid = SPRITE_MAGE_IDLE, .ttype = TILE_NODRAW, .flip = 1,                 \
    .v = { 0.0f, 0.0f }, .a = { 0.0f, 0.0f }, .health = (hp)                  \
  }

static const ENTITY enemies_template[] = {
  ENEMY (800.0, 324.0, ENEMY_HEALTH),

  ENEMY (1136.0, 298.0, ENEMY_HEALTH), ENEMY (1248.0, 266.0, ENEMY_HEALTH),

  ENEMY (1458.0, 234.0, ENEMY_HEALTH),

  ENEMY (2166.0, 324.0, ENEMY_HEALTH), ENEMY (2230.0, 282.0, ENEMY_HEALTH),
  ENEMY (2294.0, 234.0, ENEMY_HEALTH),
};

#undef ENEMY

static const ENTITY boss_template
    = { .p = { 3000.0 - SPRITE_MAGE_IDLE_W, 324.0 - SPRITE_MAGE_IDLE_H },
        .q = { 3000.0, 324.0 },
        .sz = { SPRITE_MAGE_IDLE_W, SPRITE_MAGE_IDLE_H },
        .sid = SPRITE_MAGE_IDLE,
        .ttype = TILE_NODRAW,
        .flip = 1,
        .v = { 0.0, 0.0 },
        .a = { 0.0, 0.0 },
        .health = BOSS_HEALTH };

ENTITY boss;

static unsigned char boss_shoot_timer = 0;

ENTITY enemies[sizeof enemies_template / sizeof *enemies_template];

const size_t enemies_num = sizeof enemies / sizeof *enemies;

static unsigned char shoot_timer[sizeof enemies / sizeof *enemies] = { 0 };

void
enemies_init ()
{
  memcpy (enemies, enemies_template, sizeof enemies_template);
  memset (shoot_timer, 0, sizeof shoot_timer);

  memcpy (&boss, &boss_template, sizeof boss);
  boss_shoot_timer = 0;
}

void
enemies_update (ENTITY *duck)
{
  ENTITY *e;
  float duck_cx, duck_cy;
  float boss_cx, boss_cy;
  float enemy_cx, enemy_cy;
  float dx, dy, dist;
  float angle;
  float bx_center, by_center;
  float bx, by;

  // update timers
  for (size_t i = 0; i < enemies_num; i++)
    if (shoot_timer[i])
      shoot_timer[i] = (shoot_timer[i] + 1) % ENEMY_ATTACK_RATE;

  duck_cx = (duck->p.x + duck->q.x) / 2.0;
  duck_cy = (duck->p.y + duck->q.y) / 2.0;

  for (size_t i = 0; i < enemies_num; i++)
    {
      e = &enemies[i];

      if (e->health == 0)
        {
          if (e->sid != SPRITE_MAGE_DEATH)
            {
              e->sid = SPRITE_MAGE_DEATH;

              e->q.x += (SPRITE_MAGE_DEATH_W - e->sz.x) / 2.0;

              e->sz.x = SPRITE_MAGE_DEATH_W;
              e->sz.y = SPRITE_MAGE_DEATH_H;

              e->p.x = e->q.x - e->sz.x;
              e->p.y = e->q.y - e->sz.y;

              e->flip = !e->flip;
            }

          continue;
        }

      enemy_cx = (e->p.x + e->q.x) / 2.0;
      enemy_cy = (e->p.y + e->q.y) / 2.0;

      // look at the duck
      if (duck_cx < enemy_cx)
        e->flip = 1;

      else
        e->flip = 0;

      // attack if in range
      dx = duck_cx - enemy_cx;
      dy = duck_cy - enemy_cy;
      dist = hypotf (dx, dy);

      if (dist <= ENEMY_ATTACK_RADIUS && !shoot_timer[i] && !end)
        {
          if (e->flip == 0)
            bx = e->q.x + e->v.x;

          else
            bx = e->p.x + e->v.x - SPRITE_MAGE_BULLET_W;

          by = (e->p.y + e->q.y - SPRITE_MAGE_BULLET_H) / 2.0;

          bx_center = bx + (SPRITE_MAGE_BULLET_W * 0.5f);
          by_center = by + (SPRITE_MAGE_BULLET_H * 0.5f);

          angle = atan2f (-(duck_cy - by_center), fabsf (duck_cx - bx_center))
                  / (M_PI / 2.0f);

          bullet_create (bx, by, SPRITE_MAGE_BULLET, e->flip, angle);

          shoot_timer[i] = 1;
        }
    }

  if (boss.health > 0)
    {
      duck_cx = (duck->p.x + duck->q.x) / 2.0f;
      duck_cy = (duck->p.y + duck->q.y) / 2.0f;
      boss_cx = (boss.p.x + boss.q.x) / 2.0f;
      boss_cy = (boss.p.y + boss.q.y) / 2.0f;

      if (duck_cx < boss_cx)
        boss.flip = 1;
      else
        boss.flip = 0;

      if (boss_shoot_timer)
        boss_shoot_timer = (boss_shoot_timer + 1) % BOSS_ATTACK_RATE;

      dx = duck_cx - boss_cx;
      dy = duck_cy - boss_cy;
      dist = hypotf (dx, dy);

      if (dist <= ENEMY_ATTACK_RADIUS && !boss_shoot_timer && !end)
        {
          if (boss.flip == 0)
            bx = boss.q.x + boss.v.x;
          else
            bx = boss.p.x + boss.v.x - SPRITE_MAGE_BULLET_W;

          by = (boss.p.y + boss.q.y - SPRITE_MAGE_BULLET_H) / 2.0;

          bx_center = bx + SPRITE_MAGE_BULLET_W * 0.5f;
          by_center = by + SPRITE_MAGE_BULLET_H * 0.5f;

          angle = atan2f (-(duck_cy - by_center), fabsf (duck_cx - bx_center))
                  / (M_PI / 2.0f);

          bullet_create (bx, by, SPRITE_MAGE_BULLET, boss.flip, angle);

          boss_shoot_timer = 1;
        }
    }

  else if (boss.sid != SPRITE_MAGE_DEATH)
    {
      boss.sid = SPRITE_MAGE_DEATH;

      boss.q.x += (SPRITE_MAGE_DEATH_W - boss.sz.x) / 2.0;

      boss.sz.x = SPRITE_MAGE_DEATH_W;
      boss.sz.y = SPRITE_MAGE_DEATH_H;

      boss.p.x = boss.q.x - boss.sz.x;
      boss.p.y = boss.q.y - boss.sz.y;

      boss.flip = !boss.flip;
    }
}

void
enemies_draw (CAMERA *cam, SPRITES *sprites)
{
  ALLEGRO_COLOR tint;
  ALLEGRO_BITMAP *boss_bmp;
  float x, y;

  for (size_t i = 0; i < enemies_num; i++)
    obj_draw ((OBJECT *)&enemies[i], cam, sprites);

  boss_bmp = sprites_get (sprites, boss.sid);

  x = roundf (boss.p.x - cam->offx);
  y = roundf (boss.p.y);

  tint = al_map_rgb (255, 64, 64);

  al_draw_tinted_bitmap (boss_bmp, tint, x, y, boss.flip);
}

void
enemies_bullets_hit ()
{
  for (size_t i = 0; i < enemies_num; i++)
    bullets_hit (&enemies[i]);

  bullets_hit (&boss);
}
