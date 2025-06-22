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
  // PLATFORM (300.0, 299.0, 64.0, 16.0, TILE_DIRT),
};

#undef PLATFORM

const size_t platforms_num = sizeof platforms / sizeof *platforms;

void
platforms_draw (CAMERA *cam, SPRITES *sprites)
{
  for (size_t i = 0; i < platforms_num; i++)
    obj_draw (&platforms[i], cam, sprites);
}
