#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <time.h>

#include "lib/common.h"

#define FPS 30.0
#define RES_X 640
#define RES_Y 480

/* (x1, y1) ----------+
 * |                  |
 * |                  |
 * +-----------(x2, y2) */
typedef struct OBJECT
{
  int x1, y1;   // upper left point
  int x2, y2;   // lower right point
  int xsz, ysz; // x2 - x1, y2 - y1
  int vx, vy;   // movement speed vector
  ALLEGRO_COLOR color;
} OBJECT;

int
main ()
{
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_DISPLAY *disp;
  ALLEGRO_EVENT event;

  ALLEGRO_FONT *font;
  OBJECT obj;

  bool redraw = true;
  bool done = false;

  srand (time (NULL));
  ensure (al_init ());
  ensure (al_install_keyboard ());
  ensure (al_init_image_addon ());

  ensure (font = al_create_builtin_font ());

  obj.xsz = (RES_X / 10) + rand () % (RES_X / 10);
  obj.ysz = (RES_Y / 10) + rand () % (RES_Y / 10);
  obj.x1 = rand () % (RES_X - obj.xsz);
  obj.y1 = rand () % (RES_Y - obj.ysz);
  obj.x2 = obj.x1 + obj.xsz;
  obj.y2 = obj.y1 + obj.ysz;
  obj.vx = rand () % (RES_X / 70);
  obj.vy = rand () % (RES_Y / 70);
  obj.color = al_map_rgb (0, 0, 255);

  ensure (timer = al_create_timer (1.0 / FPS));
  ensure (queue = al_create_event_queue ());
  ensure (disp = al_create_display (RES_X, RES_Y));

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

          else if (obj.x2 > RES_X)
            {
              obj.x2 = 2 * RES_X - obj.x2;
              obj.x1 = obj.x2 - obj.xsz;
              obj.vx *= -1;
            }

          if (obj.y1 < 0)
            {
              obj.y1 *= -1;
              obj.y2 = obj.y1 + obj.ysz;
              obj.vy *= -1;
            }

          else if (obj.y2 > RES_Y)
            {
              obj.y2 = 2 * RES_Y - obj.y2;
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

          al_draw_text (font, al_map_rgb (255, 255, 255), 10, 10,
                        ALLEGRO_ALIGN_LEFT, "pato sapato v0.1");
          al_draw_filled_rectangle (obj.x1, obj.y1, obj.x2, obj.y2, obj.color);

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
