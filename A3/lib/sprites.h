#ifndef SPRITES_H
#define SPRITES_H 1

#include <allegro5/allegro5.h>

#include "game.h"

#define SPRITES_UPDATE_RATE (int)(3 * (FPS / 30)) // roughly 10 FPS

#define SPRITE_DUCK_IDLE_PATH "assets/duck/idle.png"
#define SPRITE_DUCK_IDLE_W 23
#define SPRITE_DUCK_IDLE_H 20
#define SPRITE_DUCK_IDLE_STATES 4
#define SPRITE_DUCK_IDLE_CYCLIC 1

#define SPRITE_DUCK_WALK_PATH "assets/duck/walk.png"
#define SPRITE_DUCK_WALK_W 25
#define SPRITE_DUCK_WALK_H 19
#define SPRITE_DUCK_WALK_STATES 4
#define SPRITE_DUCK_WALK_CYCLIC 1

#define SPRITE_DUCK_ROLL_PATH "assets/duck/roll.png"
#define SPRITE_DUCK_ROLL_W 21
#define SPRITE_DUCK_ROLL_H 20
#define SPRITE_DUCK_ROLL_STATES 4
#define SPRITE_DUCK_ROLL_CYCLIC 1

#define SPRITE_DUCK_JUMP_PATH "assets/duck/jump.png"
#define SPRITE_DUCK_JUMP_W 25
#define SPRITE_DUCK_JUMP_H 21
#define SPRITE_DUCK_JUMP_STATES 2
#define SPRITE_DUCK_JUMP_CYCLIC 0

#define SPRITE_DUCK_FALL_PATH "assets/duck/fall.png"
#define SPRITE_DUCK_FALL_W 25
#define SPRITE_DUCK_FALL_H 20
#define SPRITE_DUCK_FALL_STATES 1
#define SPRITE_DUCK_FALL_CYCLIC 0

#define SPRITE_DUCK_CROUCH_PATH "assets/duck/crouch.png"
#define SPRITE_DUCK_CROUCH_W 34
#define SPRITE_DUCK_CROUCH_H 9
#define SPRITE_DUCK_CROUCH_STATES 1
#define SPRITE_DUCK_CROUCH_CYCLIC 0

#define SPRITE_DUCK_CRAWL_PATH "assets/duck/crawl.png"
#define SPRITE_DUCK_CRAWL_W 34
#define SPRITE_DUCK_CRAWL_H 9
#define SPRITE_DUCK_CRAWL_STATES 4
#define SPRITE_DUCK_CRAWL_CYCLIC 1

#define SPRITE_DUCK_DEATH_PATH "assets/duck/death.png"
#define SPRITE_DUCK_DEATH_W 27
#define SPRITE_DUCK_DEATH_H 16
#define SPRITE_DUCK_DEATH_STATES 1
#define SPRITE_DUCK_DEATH_CYCLIC 0

#define SPRITE_DUCK_WALL_HIT_PATH "assets/duck/wall_hit.png"
#define SPRITE_DUCK_WALL_HIT_W 15
#define SPRITE_DUCK_WALL_HIT_H 28
#define SPRITE_DUCK_WALL_HIT_STATES 1
#define SPRITE_DUCK_WALL_HIT_CYCLIC 0

#define SPRITE_DUCK_QUACK_PATH "assets/duck/quack.png"
#define SPRITE_DUCK_QUACK_W 9
#define SPRITE_DUCK_QUACK_H 29
#define SPRITE_DUCK_QUACK_STATES 4
#define SPRITE_DUCK_QUACK_CYCLIC 1

#define SPRITE_MAGE_IDLE_PATH "assets/mage/idle.png"
#define SPRITE_MAGE_IDLE_W 17
#define SPRITE_MAGE_IDLE_H 16
#define SPRITE_MAGE_IDLE_STATES 6
#define SPRITE_MAGE_IDLE_CYCLIC 1

#define SPRITE_MAGE_DEATH_PATH "assets/mage/death.png"
#define SPRITE_MAGE_DEATH_W 16
#define SPRITE_MAGE_DEATH_H 8
#define SPRITE_MAGE_DEATH_STATES 1
#define SPRITE_MAGE_DEATH_CYCLIC 0

#define SPRITE_MAGE_BULLET_PATH "assets/mage/bullet.png"
#define SPRITE_MAGE_BULLET_W 14
#define SPRITE_MAGE_BULLET_H 14
#define SPRITE_MAGE_BULLET_STATES 4
#define SPRITE_MAGE_BULLET_CYCLIC 1

typedef enum SPRITE_ID
{
  SPRITE_DUCK_IDLE = 0,
  SPRITE_DUCK_WALK,
  SPRITE_DUCK_ROLL,
  SPRITE_DUCK_JUMP,
  SPRITE_DUCK_FALL,
  SPRITE_DUCK_CROUCH,
  SPRITE_DUCK_CRAWL,
  SPRITE_DUCK_DEATH,
  SPRITE_DUCK_WALL_HIT,
  SPRITE_DUCK_QUACK,
  SPRITE_MAGE_IDLE,
  SPRITE_MAGE_DEATH,
  SPRITE_MAGE_BULLET,
  SPRITES_NUM,
  SPRITE_NODRAW
} SPRITE_ID;

typedef struct SPRITE
{
  ALLEGRO_BITMAP *sheet;     // sheet that can contain multiple sprites
  ALLEGRO_BITMAP **bmp;      // sheet sub bitmaps
  int w, h;                  // width and height of each sprite
  unsigned char state;       // current state
  unsigned char state_count; // number of states
  unsigned char cyclic;      // 1 = repeat after reaching final state
} SPRITE;

typedef struct SPRITES
{
  SPRITE spv[SPRITES_NUM];     // sprite array
  unsigned char frame_counter; // timer for the animations framerate
} SPRITES;

SPRITES *sprites_load ();

void sprites_destroy (SPRITES *sprites);

/* update frame_counter and states */
void sprites_update (SPRITES *sprites);

ALLEGRO_BITMAP *sprites_get (SPRITES *sprites, SPRITE_ID id);

void sprites_reset_state (SPRITES *sprites, SPRITE_ID id);

#endif /* sprites.h */
