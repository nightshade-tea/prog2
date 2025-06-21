#include "platforms.h"
#include "camera.h"
#include "object.h"
#include "sprites.h"

OBJECT platforms[] = { { .p = { 50.0, 200.0 },
                         .q = { 150.0, 250.0 },
                         .sz = { 100.0, 50.0 },
                         .sid = SPRITE_NODRAW,
                         .flip = 0 } };

const size_t platforms_num = sizeof platforms / sizeof *platforms;

void
platforms_draw (CAMERA *cam, SPRITES *sprites)
{
  for (size_t i = 0; i < platforms_num; i++)
    obj_draw (&platforms[i], cam, sprites);
}
