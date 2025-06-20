#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "camera.h"
#include "common.h"
#include "duck.h"
#include "keyboard.h"
#include "sprites.h"

extern ALLEGRO_TIMER *timer;
extern ALLEGRO_EVENT_QUEUE *queue;
extern ALLEGRO_DISPLAY *disp;
extern ALLEGRO_FONT *font;

void
start_game ()
{
  KEYBOARD key[ALLEGRO_KEY_MAX];
  ALLEGRO_EVENT event;
  CAMERA *cam;
  ENTITY *duck;
  SPRITES *sprites;
  bool redraw = true;
  bool done = false;

  kbd_init (key);
  ensure (cam = cam_create ());
  ensure (duck = duck_create ());
  ensure (sprites = sprites_load ());

  al_start_timer (timer);

  while (!done)
    {
      al_wait_for_event (queue, &event);

      switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
          if (key[ALLEGRO_KEY_ESCAPE] || key[ALLEGRO_KEY_Q])
            {
              done = true;
              continue;
            }

          sprites_update (sprites);
          duck_update (duck, key, sprites, cam);
          cam_move (cam, (duck->p.x + duck->q.x - RENDER_WIDTH) / 2);
          cam_update_time (cam);

          kbd_reset_seen (key);
          cam->offx += 1 / (FPS / 30);

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
          cam_draw (cam);
          obj_draw ((OBJECT *)duck, cam, sprites);

#if DBG
          al_draw_textf (font, al_map_rgb (255, 255, 255), 0, 0,
                         ALLEGRO_ALIGN_LEFT,
                         "p.x=%05.1f p.y=%05.1f q.x=%05.1f q.y=%05.1f",
                         duck->p.x, duck->p.y, duck->q.x, duck->q.y);
#endif

          al_flip_display ();
          redraw = false;
        }
    }

  cam_destroy (cam);
  ent_destroy (duck);
  sprites_destroy (sprites);
}
