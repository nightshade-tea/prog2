#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <math.h>

#include "lib/common.h"
#include "lib/def.h"
#include "lib/duck.h"
#include "lib/keyboard.h"

int
main ()
{
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_DISPLAY *disp;
  ALLEGRO_TRANSFORM transf;
  ALLEGRO_EVENT event;
  ALLEGRO_FONT *font;

  // -------------------------------------------------------------------------
  ALLEGRO_BITMAP *idle;
  unsigned char idle_state = 0;
  unsigned char frame_counter = 0;
  // -------------------------------------------------------------------------

  ENTITY *duck;
  KEYBOARD key[ALLEGRO_KEY_MAX];
  float scalex, scaley;
  bool redraw = true;
  bool done = false;

  ensure (duck = duck_create ());
  kbd_init (key);

  ensure (al_init ());
  ensure (al_install_keyboard ());
  ensure (al_init_font_addon ());
  ensure (al_init_image_addon ());
  ensure (al_init_primitives_addon ());

  al_set_new_display_flags (ALLEGRO_FULLSCREEN_WINDOW);

  ensure (idle = al_load_bitmap ("assets/duck/23x20/idle.png"));

  ensure (font = al_create_builtin_font ());
  ensure (timer = al_create_timer (1.0 / FPS));
  ensure (queue = al_create_event_queue ());
  ensure (disp = al_create_display (RENDER_WIDTH, RENDER_HEIGHT));

  scalex = ((float)al_get_display_width (disp)) / RENDER_WIDTH;
  scaley = ((float)al_get_display_height (disp)) / RENDER_HEIGHT;
  al_identity_transform (&transf);
  al_scale_transform (&transf, scalex, scaley);
  al_use_transform (&transf);
  al_hide_mouse_cursor (disp);

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
          duck_update_position (duck, key);
          obj_keep_inside_bounds ((OBJECT *)duck);

          if (key[ALLEGRO_KEY_ESCAPE] || key[ALLEGRO_KEY_Q])
            done = true;

          kbd_reset_seen (key);

          frame_counter++;
          frame_counter %= (int)(3 * (FPS / 30));

          if (!frame_counter)
            {
              idle_state++;
              idle_state %= 4;
            }

          redraw = true;
          break;

        case ALLEGRO_EVENT_KEY_DOWN:
          kbd_press (key, event.keyboard.keycode);
          break;

        case ALLEGRO_EVENT_KEY_UP:
          kbd_release (key, event.keyboard.keycode);
          break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          done = true;
          continue;
        }

      if (redraw && al_is_event_queue_empty (queue))
        {
          al_clear_to_color (al_map_rgb (0, 0, 0));

          al_draw_rectangle (duck->p.x, duck->p.y, duck->q.x, duck->q.y,
                             al_map_rgb (255, 0, 0), 1);

          al_draw_bitmap_region (idle, idle_state * 23, 0, 23, 20, duck->p.x,
                                 duck->p.y, 0);

          al_draw_textf (font, al_map_rgb (255, 255, 255), 0, 0,
                         ALLEGRO_ALIGN_LEFT,
                         "p.x=%05.1f p.y=%05.1f q.x=%05.1f q.y=%05.1f",
                         duck->p.x, duck->p.y, duck->q.x, duck->q.y);

          al_flip_display ();
          redraw = false;
        }
    }

  ent_destroy (duck);
  al_destroy_font (font);
  al_destroy_display (disp);
  al_destroy_timer (timer);
  al_destroy_event_queue (queue);

  return 0;
}
