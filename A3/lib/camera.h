#ifndef CAMERA_H
#define CAMERA_H 1

#include <allegro5/allegro5.h>

#define CAM_LAYERS 4
#define CAM_LAYER_WIDTH 576
#define CAM_LAYER_HEIGHT 324

#define CAM_LAYER_1_PATH "assets/backgrounds/nature_4/1.png"
#define CAM_LAYER_1_RELSPD 0.1

#define CAM_LAYER_2_PATH "assets/backgrounds/nature_4/2.png"
#define CAM_LAYER_2_RELSPD 0.2

#define CAM_LAYER_3_PATH "assets/backgrounds/nature_4/3.png"
#define CAM_LAYER_3_RELSPD 0.4

#define CAM_LAYER_4_PATH "assets/backgrounds/nature_4/4.png"
#define CAM_LAYER_4_RELSPD 0.8

#define CAM_BEGIN 0
#define CAM_END 2304

typedef struct CAMERA
{
  ALLEGRO_BITMAP *layer[CAM_LAYERS];
  float offx;
} CAMERA;

CAMERA *cam_create ();

void cam_destroy (CAMERA *cam);

void cam_move (CAMERA *cam, float offset);

void cam_draw (CAMERA *cam);

#endif /* camera.h */
