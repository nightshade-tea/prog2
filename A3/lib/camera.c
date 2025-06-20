#include <allegro5/allegro5.h>
#include <math.h>
#include <stdlib.h>

#include "camera.h"
#include "common.h"
#include "def.h"

#define load(X)                                                               \
  ensure (cam->layer[X - 1] = al_load_bitmap (CAM_LAYER_##X##_PATH))

CAMERA *
cam_create ()
{
  CAMERA *cam;

  if (!(cam = malloc (sizeof *cam)))
    return NULL;

  cam->offx = 0;
  cam->time = 0;

  load (1);
  load (2);
  load (3);
  load (4);

  return cam;
}

#undef load

void
cam_destroy (CAMERA *cam)
{
  for (int i = 0; i < CAM_LAYERS; i++)
    al_destroy_bitmap (cam->layer[i]);

  free (cam);
}

void
cam_move (CAMERA *cam, float offset)
{
  if (offset < CAM_BEGIN)
    offset = CAM_BEGIN;

  else if (offset > CAM_END - RENDER_WIDTH)
    offset = CAM_END - RENDER_WIDTH;

  cam->offx = offset;
}

void
cam_update_time (CAMERA *cam)
{
  cam->time += CAM_TIME_INC;
}

static void
draw_layer (ALLEGRO_BITMAP *layer, float offset)
{
  float x1, x2;

  x1 = fmod (offset, CAM_LAYER_WIDTH);
  x2 = CAM_LAYER_WIDTH - x1;

  al_draw_bitmap_region (layer, x1, 0, x2, CAM_LAYER_HEIGHT, 0, 0, 0);
  al_draw_bitmap_region (layer, 0, 0, CAM_LAYER_WIDTH - x2, CAM_LAYER_HEIGHT,
                         CAM_LAYER_WIDTH - x1, 0, 0);
}

#define draw(X)                                                               \
  draw_layer (cam->layer[X - 1], cam->offx *CAM_LAYER_##X##_RELSPD            \
                                     + cam->time * CAM_LAYER_##X##_BASESPD)

void
cam_draw (CAMERA *cam)
{
  draw (1);
  draw (2);
  draw (3);
  draw (4);
}

#undef draw
