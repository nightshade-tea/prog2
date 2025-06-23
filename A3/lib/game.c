#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <stdbool.h>

#include "bullets.h"
#include "camera.h"
#include "common.h"
#include "duck.h"
#include "enemies.h"
#include "game.h"
#include "keyboard.h"
#include "platforms.h"
#include "sprites.h"
#include "tiles.h"

extern ALLEGRO_TIMER *timer;
extern ALLEGRO_EVENT_QUEUE *queue;
extern ALLEGRO_DISPLAY *disp;
extern ALLEGRO_FONT *font;
extern SPRITES *sprites;
extern size_t alive_enemies;

static bool paused;
unsigned char end; // 0 = playing, 1 = game over, 2 = success

static void
success ()
{
  end = 2;
}

static void
fail ()
{
  end = 1;
}

static void
pause_game ()
{
  paused = true;
}

static void
resume_game ()
{
  paused = false;
}

static void
pause_write (char const *text, float x, float y, int flags)
{
  al_draw_text (font, al_map_rgb (PAUSE_MSG_SHADOW_COLOR), x - 1.0, y + 1.0,
                flags, text);
  al_draw_text (font, al_map_rgb (PAUSE_MSG_FOREGROUND_COLOR), x, y, flags,
                text);
}

static void
pause_menu ()
{
  al_draw_filled_rectangle (0, 0, RENDER_WIDTH, RENDER_HEIGHT,
                            al_map_rgba (PAUSE_DIM_COLOR));

  pause_write (PAUSE_MSG, PAUSE_MSG_X, PAUSE_MSG_Y, ALLEGRO_ALIGN_CENTRE);
}

static void
fail_menu ()
{
  al_draw_filled_rectangle (0, 0, RENDER_WIDTH, RENDER_HEIGHT,
                            al_map_rgba (PAUSE_DIM_COLOR));

  pause_write (GAME_OVER_MSG, PAUSE_MSG_X, PAUSE_MSG_Y, ALLEGRO_ALIGN_CENTRE);
}

static void
success_menu ()
{
  al_draw_filled_rectangle (0, 0, RENDER_WIDTH, RENDER_HEIGHT,
                            al_map_rgba (PAUSE_DIM_COLOR));

  pause_write (SUCCESS_MSG, PAUSE_MSG_X, PAUSE_MSG_Y, ALLEGRO_ALIGN_CENTRE);
}

void
start_game ()
{
  KEYBOARD key[ALLEGRO_KEY_MAX];
  ALLEGRO_EVENT event;
  CAMERA *cam;
  ENTITY *duck;
  bool redraw = true;
  bool done = false;

  paused = false;
  end = 0;

  kbd_init (key);
  tiles_init ();
  bullets_init ();
  ensure (cam = cam_create ());
  ensure (duck = duck_create ());

  al_start_timer (timer);

  while (!done)
    {
      al_wait_for_event (queue, &event);

      switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
          if (paused)
            break;

          sprites_update (sprites);
          duck_update (duck, key, sprites, cam);
          enemies_update (duck);
          bullets_update (cam);
          enemies_bullets_hit ();
          bullets_hit (duck);

          cam_move (cam, (duck->p.x + duck->q.x - RENDER_WIDTH) / 2);
          cam_update_time (cam);

          kbd_reset_seen (key);
          cam->offx += 1 / (FPS / 30);

          if (duck->sid == SPRITE_DUCK_DEATH)
            fail ();

          else if (alive_enemies == 0)
            success ();

          redraw = true;
          break;

        case ALLEGRO_EVENT_KEY_DOWN:
          kbd_press (key, event.keyboard.keycode);

          if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
              if (end)
                {
                  done = true;
                  continue;
                }

              if (!paused)
                pause_game ();

              else
                resume_game ();

              redraw = true;
            }

          if (event.keyboard.keycode == ALLEGRO_KEY_Q && (paused || end))
            {
              done = true;
              continue;
            }

          if (paused)
            kbd_reset_seen (key);

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
          cam_draw (cam);
          platforms_draw (cam, sprites);
          bullets_draw (cam, sprites);
          enemies_draw (cam, sprites);
          obj_draw ((OBJECT *)duck, cam, sprites);

#if DBG
          al_draw_textf (font, al_map_rgb (255, 255, 255), 0, 0,
                         ALLEGRO_ALIGN_LEFT,
                         "p.x=%05.1f p.y=%05.1f q.x=%05.1f q.y=%05.1f",
                         duck->p.x, duck->p.y, duck->q.x, duck->q.y);
#endif

          if (paused)
            pause_menu ();

          if (end == 1)
            fail_menu ();

          if (end == 2)
            success_menu ();

          al_flip_display ();
          redraw = false;
        }
    }

  tiles_destroy ();
  bullets_destroy ();
  cam_destroy (cam);
  ent_destroy (duck);
}
