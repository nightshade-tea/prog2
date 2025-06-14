#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "lib/common.h"

#define FPS 30.0
#define RES_X 640
#define RES_Y 480

typedef enum BOUNCER_TYPE
{
  BT_HELLO = 0,
  BT_MYSHA,
  BT_TRIANGLE,
  BT_RECTANGLE,
  BT_CIRCLE,
  BT_LINE1,
  BT_LINE2,
  BT_SZ
} BOUNCER_TYPE;

typedef struct BOUNCER
{
  float x, y;
  float dx, dy;
  BOUNCER_TYPE type;
} BOUNCER;

int
main ()
{
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_DISPLAY *disp;
  ALLEGRO_FONT *font;
  ALLEGRO_EVENT event;
  ALLEGRO_BITMAP *mysha;
  BOUNCER bvec[BT_SZ];
  BOUNCER *b;

  bool redraw = true;
  bool done = false;

  ensure (al_init ());
  ensure (al_install_keyboard ());
  ensure (al_init_image_addon ());

  ensure (font = al_create_builtin_font ());
  ensure (mysha = al_load_bitmap ("assets/mysha.png"));

  for (BOUNCER_TYPE i = 0; i < BT_SZ; i++)
    {
      b = &bvec[i];
      b->x = rand () % RES_X;
      b->y = rand () % RES_Y;
      b->dx = ((((float)rand ()) / RAND_MAX) - 0.5) * 8;
      b->dy = ((((float)rand ()) / RAND_MAX) - 0.5) * 8;
      b->type = i;
    }

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
          for (BOUNCER_TYPE i = 0; i < BT_SZ; i++)
            {
              b = &bvec[i];
              b->x += b->dx;
              b->y += b->dy;

              if (b->x < 0)
                {
                  b->x *= -1;
                  b->dx *= -1;
                }

              if (b->x > RES_X)
                {
                  b->x = 2 * RES_X - b->x;
                  b->dx *= -1;
                }

              if (b->y < 0)
                {
                  b->y *= -1;
                  b->dy *= -1;
                }

              if (b->y > RES_Y)
                {
                  b->y = 2 * RES_Y - b->y;
                  b->dy *= -1;
                }
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

          for (BOUNCER_TYPE i = 0; i < BT_SZ; i++)
            {
              b = &bvec[i];
              switch (b->type)
                {
                case BT_HELLO:
                  al_draw_text (font, al_map_rgb (255, 255, 255), b->x, b->y,
                                ALLEGRO_ALIGN_LEFT, "pato sapato v0.1");
                  break;

                case BT_MYSHA:
                  al_draw_bitmap (mysha, b->x, b->y, 0);
                  break;

                case BT_TRIANGLE:
                  al_draw_filled_triangle (b->x, b->y, b->x + 50, b->y + 25,
                                           b->x, b->y + 50,
                                           al_map_rgb_f (0, 1, 0));
                  break;

                case BT_RECTANGLE:
                  al_draw_filled_rectangle (b->x, b->y, b->x + 100, b->y + 80,
                                            al_map_rgba_f (0, 0, 0.5, 0.5));
                  break;

                case BT_CIRCLE:
                  al_draw_circle (b->x, b->y, 30, al_map_rgb_f (1, 0, 1), 2);
                  break;

                case BT_LINE1:
                  al_draw_line (b->x, b->y, b->x + 20, b->y + 100,
                                al_map_rgb_f (1, 0, 0), 1);
                  break;

                case BT_LINE2:
                  al_draw_line (b->x, b->y, b->x + 70, b->y - 20,
                                al_map_rgb_f (1, 1, 0), 1);
                  break;
                }
            }

          al_flip_display ();
          redraw = false;
        }
    }

  al_destroy_bitmap (mysha);
  al_destroy_font (font);
  al_destroy_display (disp);
  al_destroy_timer (timer);
  al_destroy_event_queue (queue);

  return 0;
}
