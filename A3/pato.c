#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "lib/common.h"

#define FPS 30.0
#define RES_X 640
#define RES_Y 480

int
main ()
{
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_DISPLAY *disp;
  ALLEGRO_FONT *font;
  ALLEGRO_EVENT event;
  ALLEGRO_BITMAP *mysha;

  bool redraw = true;
  bool done = false;

  ensure (al_init ());
  ensure (al_install_keyboard ());
  ensure (al_init_image_addon ());

  ensure (font = al_create_builtin_font ());
  ensure (mysha = al_load_bitmap ("assets/mysha.png"));

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

          al_draw_bitmap (mysha, 100, 100, 0);

          al_draw_filled_triangle (35, 350, 85, 375, 35, 400,
                                   al_map_rgb_f (0, 1, 0));
          al_draw_filled_rectangle (240, 260, 340, 340,
                                    al_map_rgba_f (0, 0, 0.5, 0.5));
          al_draw_circle (450, 370, 30, al_map_rgb_f (1, 0, 1), 2);
          al_draw_line (440, 110, 460, 210, al_map_rgb_f (1, 0, 0), 1);
          al_draw_line (500, 220, 570, 200, al_map_rgb_f (1, 1, 0), 1);

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
