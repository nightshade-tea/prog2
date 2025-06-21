#include "platforms.h"
#include "camera.h"
#include "object.h"
#include "sprites.h"

OBJECT platforms[] = { { .p = { 250.0, 274.0 },
                         .q = { 350.0, 314.0 },
                         .sz = { 100.0, 40.0 },
                         .sid = SPRITE_NODRAW,
                         .flip = 0 } };

const size_t platforms_num = sizeof platforms / sizeof *platforms;

void
platforms_draw (CAMERA *cam, SPRITES *sprites)
{
  for (size_t i = 0; i < platforms_num; i++)
    obj_draw (&platforms[i], cam, sprites);
}
