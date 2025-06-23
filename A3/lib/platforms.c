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
  PLATFORM (1088.0, 298.0, 96.0, 32.0, TILE_GRASS),
  PLATFORM (1184.0, 266.0, 96.0, 64.0, TILE_GRASS),

  PLATFORM (1314.0, 202.0, 80.0, 112.0, TILE_GRASS),
  PLATFORM (1394.0, 234.0, 128.0, 80.0, TILE_GRASS),

  PLATFORM (2198.0, 282.0, 64.0, 48.0, TILE_GRASS),
  PLATFORM (2262.0, 234.0, 64.0, 96.0, TILE_GRASS),
};

#undef PLATFORM

const size_t platforms_num = sizeof platforms / sizeof *platforms;

void
platforms_draw (CAMERA *cam, SPRITES *sprites)
{
  for (size_t i = 0; i < platforms_num; i++)
    obj_draw (&platforms[i], cam, sprites);
}
