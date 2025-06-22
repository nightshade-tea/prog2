#include "platforms.h"
#include "camera.h"
#include "object.h"
#include "sprites.h"
#include "tiles.h"

#define PLATFORM(px, py, w, h, ttype_)                                        \
  { .p = { (px), (py) },                                                      \
    .q = { (px) + (w), (py) + (h) },                                          \
    .sz = { (w), (h) },                                                       \
    .sid = SPRITE_NODRAW,                                                     \
    .ttype = (ttype_),                                                        \
    .flip = 0 }

OBJECT platforms[] = {
  /* ground */
  PLATFORM (-8.0, 304.0, 2324.0, 32.0, TILE_GRASS),

  PLATFORM (300.0, 279.0, 64.0, 16.0, TILE_GRASS),
};

#undef PLATFORM

const size_t platforms_num = sizeof platforms / sizeof *platforms;

void
platforms_draw (CAMERA *cam, SPRITES *sprites)
{
  for (size_t i = 0; i < platforms_num; i++)
    obj_draw (&platforms[i], cam, sprites);
}
