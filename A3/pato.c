#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include "common.h"

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

  bool redraw = true;
  bool done = false;

  ensure (al_init ());
  ensure (al_install_keyboard ());

  ensure (timer = al_create_timer (1.0 / FPS));
  ensure (queue = al_create_event_queue ());
  ensure (disp = al_create_display (RES_X, RES_Y));
  ensure (font = al_create_builtin_font ());

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
          al_clear_to_color (al_map_rgb (0, 0, 150));
          al_draw_text (font, al_map_rgb (255, 255, 255), RES_X / 2, RES_Y / 2,
                        ALLEGRO_ALIGN_CENTRE, "pato sapato v0.1");
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
