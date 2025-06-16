#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <time.h>

#include "lib/common.h"

#define FPS 60.0
#define RENDER_WIDTH 320
#define RENDER_HEIGHT 200

/* (x1, y1) ----------+
 * |                  |
 * |                  |
 * +-----------(x2, y2) */
typedef struct OBJECT
{
  float x1, y1;         // upper left point
  float x2, y2;         // lower right point
  float xsz, ysz;       // x2 - x1, y2 - y1
  float vx, vy;         // movement speed vector
  float thck;           // border thickness
  ALLEGRO_COLOR fill;   // fill color
  ALLEGRO_COLOR border; // border color
} OBJECT;

void
init_obj (OBJECT *obj)
{
  obj->xsz = (RENDER_WIDTH / 20) + rand () % (RENDER_WIDTH / 20);
  obj->ysz = (RENDER_HEIGHT / 20) + rand () % (RENDER_HEIGHT / 20);

  obj->x1 = rand () % (RENDER_WIDTH - (int)obj->xsz);
  obj->y1 = rand () % (RENDER_HEIGHT - (int)obj->ysz);

  obj->x2 = obj->x1 + obj->xsz;
  obj->y2 = obj->y1 + obj->ysz;

  obj->vx = (RENDER_WIDTH / 140) + rand () % (RENDER_WIDTH / 140);
  obj->vy = (RENDER_HEIGHT / 140) + rand () % (RENDER_HEIGHT / 140);
  obj->vx /= FPS / 30;
  obj->vy /= FPS / 30;

  obj->thck = 1;
  obj->fill = al_map_rgb (50, 50, 50);
  obj->border = al_map_rgb (255, 0, 0);
}

int
main ()
{
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_DISPLAY *disp;
  ALLEGRO_TRANSFORM transf;
  ALLEGRO_EVENT event;
  ALLEGRO_FONT *font;

  OBJECT obj;
  float scalex, scaley;
  bool redraw = true;
  bool done = false;

  srand (time (NULL));
  init_obj (&obj);

  ensure (al_init ());
  ensure (al_install_keyboard ());
  ensure (al_init_image_addon ());

  al_set_new_display_flags (ALLEGRO_FULLSCREEN_WINDOW);
//al_set_new_display_option (ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);

  ensure (font = al_create_builtin_font ());
  ensure (timer = al_create_timer (1.0 / FPS));
  ensure (queue = al_create_event_queue ());
  ensure (disp = al_create_display (RENDER_WIDTH, RENDER_HEIGHT));

  scalex = ((float)al_get_display_width (disp)) / RENDER_WIDTH;
  scaley = ((float)al_get_display_height (disp)) / RENDER_HEIGHT;
  al_identity_transform (&transf);
  al_scale_transform (&transf, scalex, scaley);
  al_use_transform (&transf);

  al_register_event_source (queue, al_get_keyboard_event_source ());
  al_register_event_source (queue, al_get_display_event_source (disp));
  al_register_event_source (queue, al_get_timer_event_source (timer));

  al_start_timer (timer);

  while (!done)
    {
      al_wait_for_event (queue, &event);

      switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
          obj.x1 += obj.vx;
          obj.y1 += obj.vy;
          obj.x2 += obj.vx;
          obj.y2 += obj.vy;

          if (obj.x1 < 0)
            {
              obj.x1 *= -1;
              obj.x2 = obj.x1 + obj.xsz;
              obj.vx *= -1;
            }

          else if (obj.x2 > RENDER_WIDTH)
            {
              obj.x2 = 2 * RENDER_WIDTH - obj.x2;
              obj.x1 = obj.x2 - obj.xsz;
              obj.vx *= -1;
            }

          if (obj.y1 < 0)
            {
              obj.y1 *= -1;
              obj.y2 = obj.y1 + obj.ysz;
              obj.vy *= -1;
            }

          else if (obj.y2 > RENDER_HEIGHT)
            {
              obj.y2 = 2 * RENDER_HEIGHT - obj.y2;
              obj.y1 = obj.y2 - obj.ysz;
              obj.vy *= -1;
            }

          redraw = true;
          break;

        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          done = true;
          continue;
        }

      if (redraw && al_is_event_queue_empty (queue))
        {
          al_clear_to_color (al_map_rgb (0, 0, 0));
          al_draw_filled_rectangle (obj.x1, obj.y1, obj.x2, obj.y2, obj.fill);
          al_draw_rectangle (obj.x1, obj.y1, obj.x2, obj.y2, obj.border,
                             obj.thck);
          al_draw_text (font, al_map_rgb (255, 255, 255), 10, 10,
                        ALLEGRO_ALIGN_LEFT, "pato sapato v0.1");
          al_flip_display ();
          redraw = false;
        }
    }

  al_destroy_font (font);
  al_destroy_display (disp);
  al_destroy_timer (timer);
  al_destroy_event_queue (queue);

  return 0;
}
