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
                         SPRITE_DUCK_IDLE_W, SPRITE_DUCK_IDLE_H, MENU_DUCK_X,
                         MENU_DUCK_Y, SPRITE_DUCK_IDLE_W * MENU_DUCK_SCALE,
                         SPRITE_DUCK_IDLE_H * MENU_DUCK_SCALE, 0);
}

static void
draw_selected (char const *text, float x, float y, int flags)
{
  al_draw_text (font, al_map_rgb (MENU_SELECT_COLOR), x - 1.0, y + 1.0, flags,
                text);
  al_draw_text (font, al_map_rgb (MENU_FOREGROUND_COLOR), x, y, flags, text);
}

static void
draw_regular (char const *text, float x, float y, int flags)
{
  al_draw_text (font, al_map_rgb (MENU_SHADOW_COLOR), x - 1.0, y + 1.0, flags,
                text);
  al_draw_text (font, al_map_rgb (MENU_FOREGROUND_COLOR), x, y, flags, text);
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

          if (!(frame_counter % MENU_DOT_UPDATE_RATE))
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
          al_clear_to_color (al_map_rgb (MENU_BACKGROUND_COLOR));

          draw_regular (MENU_TITLE, MENU_TITLE_X, MENU_TITLE_Y,
                        ALLEGRO_ALIGN_CENTRE);

          draw_duck ();

          if (start)
            {
              if (dot)
                {
                  draw_selected (MENU_START_DOT, MENU_START_X, MENU_START_Y,
                                 ALLEGRO_ALIGN_CENTRE);
                  draw_regular (MENU_EXIT, MENU_EXIT_X, MENU_EXIT_Y,
                                ALLEGRO_ALIGN_CENTRE);
                }

              else
                {
                  draw_selected (MENU_START, MENU_START_X, MENU_START_Y,
                                 ALLEGRO_ALIGN_CENTRE);
                  draw_regular (MENU_EXIT, MENU_EXIT_X, MENU_EXIT_Y,
                                ALLEGRO_ALIGN_CENTRE);
                }
            }

          else
            {
              if (dot)
                {
                  draw_regular (MENU_START, MENU_START_X, MENU_START_Y,
                                ALLEGRO_ALIGN_CENTRE);
                  draw_selected (MENU_EXIT_DOT, MENU_EXIT_X, MENU_EXIT_Y,
                                 ALLEGRO_ALIGN_CENTRE);
                }

              else
                {
                  draw_regular (MENU_START, MENU_START_X, MENU_START_Y,
                                ALLEGRO_ALIGN_CENTRE);
                  draw_selected (MENU_EXIT, MENU_EXIT_X, MENU_EXIT_Y,
                                 ALLEGRO_ALIGN_CENTRE);
                }
            }

          al_flip_display ();
          redraw = false;
        }
    }

  al_stop_timer (timer);

  return start;
}
