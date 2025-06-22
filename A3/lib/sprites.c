#include <allegro5/allegro5.h>

#include "common.h"
#include "sprites.h"

static int
sprite_init (SPRITE *s, const char *path, int w, int h, unsigned char states,
             unsigned char cyclic)
{
  if (!(s->sheet = al_load_bitmap (path)))
    return 1;

  if (!(s->bmp = malloc (states * sizeof *s->bmp)))
    return 1;

  for (int i = 0; i < states; i++)
    if (!(s->bmp[i] = al_create_sub_bitmap (s->sheet, i * w, 0, w, h)))
      return 1;

  s->w = w;
  s->h = h;
  s->state = 0;
  s->state_count = states;
  s->cyclic = cyclic;

  return 0;
}

#define init(X)                                                               \
  ensure (!sprite_init (&(sps->spv[SPRITE_##X]), SPRITE_##X##_PATH,           \
                        SPRITE_##X##_W, SPRITE_##X##_H, SPRITE_##X##_STATES,  \
                        SPRITE_##X##_CYCLIC))

SPRITES *
sprites_load ()
{
  SPRITES *sps;

  if (!(sps = malloc (sizeof *sps)))
    return NULL;

  sps->frame_counter = 0;

  init (DUCK_IDLE);
  init (DUCK_WALK);
  init (DUCK_ROLL);
  init (DUCK_JUMP);
  init (DUCK_FALL);
  init (DUCK_CROUCH);
  init (DUCK_CRAWL);
  init (DUCK_DEATH);
  init (DUCK_WALL_HIT);

  init (DUCK_QUACK);
  init (MAGE_IDLE);
  init (MAGE_DEATH);
  init (MAGE_BULLET);

  return sps;
}

#undef init

void
sprites_destroy (SPRITES *sprites)
{
  SPRITE *s;

  for (int i = 0; i < SPRITES_NUM; i++)
    {
      s = &(sprites->spv[i]);

      al_destroy_bitmap (s->sheet);

      for (int j = 0; j < s->state_count; j++)
        al_destroy_bitmap (s->bmp[j]);

      free (s->bmp);
    }

  free (sprites);
}

void
sprites_update (SPRITES *sprites)
{
  SPRITE *s;

  sprites->frame_counter++;
  sprites->frame_counter %= SPRITES_UPDATE_RATE;

  if (sprites->frame_counter)
    return;

  for (int i = 0; i < SPRITES_NUM; i++)
    {
      s = &(sprites->spv[i]);

      s->state++;

      if (s->cyclic)
        s->state %= s->state_count;

      else if (s->state >= s->state_count)
        s->state = s->state_count - 1;
    }
}

ALLEGRO_BITMAP *
sprites_get (SPRITES *sprites, SPRITE_ID id)
{
  SPRITE *s = &(sprites->spv[id]);

  return s->bmp[s->state];
}

void
sprites_reset_state (SPRITES *sprites, SPRITE_ID id)
{
  sprites->spv[id].state = 0;
}
