#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <stdbool.h>

#include "game.h"
#include "keyboard.h"
#include "menu.h"
#include "sprites.h"

extern ALLEGRO_TIMER *timer;
extern ALLEGRO_EVENT_QUEUE *queue;
extern ALLEGRO_DISPLAY *disp;
extern ALLEGRO_FONT *font;
extern SPRITES *sprites;

static void
draw_duck ()
{
  al_draw_scaled_bitmap (sprites_get (sprites, SPRITE_DUCK_IDLE), 0, 0,
                         SPRITE_DUCK_IDLE_W, SPRITE_DUCK_IDLE_H, DUCK_X,
                         DUCK_Y, SPRITE_DUCK_IDLE_W * DUCK_SCALE,
                         SPRITE_DUCK_IDLE_H * DUCK_SCALE, 0);
}

static void
draw_selected (char const *text, float x, float y, int flags)
{
  al_draw_text (font, al_map_rgb (SELECT_COLOR), x - 1.0, y + 1.0, flags,
                text);
  al_draw_text (font, al_map_rgb (FOREGROUND_COLOR), x, y, flags, text);
}

static void
draw_regular (char const *text, float x, float y, int flags)
{
  al_draw_text (font, al_map_rgb (SHADOW_COLOR), x - 1.0, y + 1.0, flags,
                text);
  al_draw_text (font, al_map_rgb (FOREGROUND_COLOR), x, y, flags, text);
}

unsigned char
start_menu ()
{
  KEYBOARD key[ALLEGRO_KEY_MAX];
  ALLEGRO_EVENT event;
  bool redraw = true;
  bool done = false;
  bool dot = false;
  unsigned char frame_counter = 0;
  unsigned char start = 1;

  kbd_init (key);
  al_start_timer (timer);

  while (!done)
    {
      al_wait_for_event (queue, &event);

      switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
          sprites_update (sprites);

          frame_counter++;

          if (!(frame_counter % DOT_UPDATE_RATE))
            {
              dot = !dot;
              frame_counter = 0;
            }

          if (key[ALLEGRO_KEY_W] || key[ALLEGRO_KEY_UP])
            {
              start = 1;
              dot = true;
              frame_counter = 0;
            }

          if (key[ALLEGRO_KEY_S] || key[ALLEGRO_KEY_DOWN])
            {
              start = 0;
              dot = true;
              frame_counter = 0;
            }

          if (key[ALLEGRO_KEY_ENTER])
            {
              done = true;
              continue;
            }

          kbd_reset_seen (key);

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
          start = 0;
          continue;
        }

      if (redraw && al_is_event_queue_empty (queue))
        {
          al_clear_to_color (al_map_rgb (BACKGROUND_COLOR));

          draw_regular (TITLE, TITLE_X, TITLE_Y, ALLEGRO_ALIGN_CENTRE);

          draw_duck ();

          if (start)
            {
              if (dot)
                {
                  draw_selected (START_DOT, START_X, START_Y,
                                 ALLEGRO_ALIGN_CENTRE);
                  draw_regular (EXIT, EXIT_X, EXIT_Y, ALLEGRO_ALIGN_CENTRE);
                }

              else
                {
                  draw_selected (START, START_X, START_Y,
                                 ALLEGRO_ALIGN_CENTRE);
                  draw_regular (EXIT, EXIT_X, EXIT_Y, ALLEGRO_ALIGN_CENTRE);
                }
            }

          else
            {
              if (dot)
                {
                  draw_regular (START, START_X, START_Y, ALLEGRO_ALIGN_CENTRE);
                  draw_selected (EXIT_DOT, EXIT_X, EXIT_Y,
                                 ALLEGRO_ALIGN_CENTRE);
                }

              else
                {
                  draw_regular (START, START_X, START_Y, ALLEGRO_ALIGN_CENTRE);
                  draw_selected (EXIT, EXIT_X, EXIT_Y, ALLEGRO_ALIGN_CENTRE);
                }
            }

          al_flip_display ();
          redraw = false;
        }
    }

  al_stop_timer (timer);

  return start;
}
