#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <math.h>

#include "lib/common.h"
#include "lib/keyboard.h"

#define RENDER_WIDTH 480
#define RENDER_HEIGHT 300
#define FPS 60.0
#define GRAV 2 / (FPS / 30)

typedef struct PAIR
{
  float x, y;
} PAIR;

/* p ----------+
 * | \         |
 * |   \       |
 * |    sz     |
 * |       \   |
 * |         \ |
 * +-----------q */
typedef struct OBJECT
{
  PAIR p, q; // upper left and bottom right points
  PAIR sz;   // size vector (q.x - p.x, q.y - p.y)
  PAIR vel;  // velocity vector
  PAIR acc;  // acceleration vector

  float crouchspd; // crouched speed
  float walkspd;   // walking speed
  float runspd;    // running speed
  float slowdown;  // x axis slowdown acceleration
  float jmpspd;    // jump speed
  float glidespd;  // plane speed
  float thck;      // border thickness

  ALLEGRO_COLOR fill;   // fill color
  ALLEGRO_COLOR border; // border color
} OBJECT;

void
obj_init (OBJECT *obj)
{
  obj->sz.x = 23;
  obj->sz.y = 20;

  obj->p.x = (RENDER_WIDTH - obj->sz.x) / 2;
  obj->p.y = (RENDER_HEIGHT - obj->sz.y) / 2;

  obj->q.x = obj->p.x + obj->sz.x;
  obj->q.y = obj->p.y + obj->sz.y;

  obj->vel.x = obj->vel.y = 0;
  obj->acc.x = 0;
  obj->acc.y = GRAV;

  obj->runspd = (obj->sz.x * 0.75) / (FPS / 30);
  obj->walkspd = obj->runspd / 2;
  obj->crouchspd = obj->runspd / 4;

  obj->slowdown = obj->walkspd / 20;
  obj->jmpspd = (obj->sz.y * 1.25) / (FPS / 30);
  obj->glidespd = GRAV * 2;

  obj->thck = 1;
  obj->fill = al_map_rgb (0, 0, 255);
  obj->border = al_map_rgb (255, 0, 0);
}

void
obj_update_position (OBJECT *obj, KEYBOARD key[ALLEGRO_KEY_MAX])
{
  if (fabs (obj->vel.x) <= 2 * obj->slowdown)
    {
      obj->acc.x = 0;
      obj->vel.x = 0;
    }
  else if (obj->vel.x > 0)
    obj->acc.x = -obj->slowdown;
  else
    obj->acc.x = obj->slowdown;

  if (obj->vel.y >= obj->glidespd && (key[ALLEGRO_KEY_SPACE] & KEY_DOWN))
    {
      obj->acc.y = 0;
      obj->vel.y = obj->glidespd;
    }
  else
    obj->acc.y = GRAV;

  obj->vel.x += obj->acc.x;
  obj->vel.y += obj->acc.y;

  if (key[ALLEGRO_KEY_SPACE] & KEY_SEEN)
    obj->vel.y = -obj->jmpspd;

  if (key[ALLEGRO_KEY_A])
    obj->vel.x = -obj->walkspd;

  if (key[ALLEGRO_KEY_D])
    obj->vel.x = obj->walkspd;

  obj->p.x += obj->vel.x;
  obj->q.x += obj->vel.x;
  obj->p.y += obj->vel.y;
  obj->q.y += obj->vel.y;
}

void
obj_keep_inside_bounds (OBJECT *obj)
{
  if (obj->p.x < 0.5)
    {
      obj->p.x = 0.5;
      obj->q.x = obj->p.x + obj->sz.x;
    }

  else if (obj->q.x > RENDER_WIDTH - 0.5)
    {
      obj->q.x = RENDER_WIDTH - 0.5;
      obj->p.x = obj->q.x - obj->sz.x;
    }

  if (obj->p.y < 0.5)
    {
      obj->p.y = 0.5;
      obj->q.y = obj->p.y + obj->sz.y;
    }

  else if (obj->q.y > RENDER_HEIGHT - 0.5)
    {
      obj->q.y = RENDER_HEIGHT - 0.5;
      obj->p.y = obj->q.y - obj->sz.y;
    }
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
  KEYBOARD key[ALLEGRO_KEY_MAX];
  float scalex, scaley;
  bool redraw = true;
  bool done = false;

  obj_init (&obj);
  kbd_init (key);

  ensure (al_init ());
  ensure (al_install_keyboard ());
  ensure (al_init_font_addon ());
  ensure (al_init_image_addon ());
  ensure (al_init_primitives_addon ());

  al_set_new_display_flags (ALLEGRO_FULLSCREEN_WINDOW);
  // al_set_new_display_option (ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);

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
          obj_update_position (&obj, key);
          obj_keep_inside_bounds (&obj);

          if (key[ALLEGRO_KEY_ESCAPE] || key[ALLEGRO_KEY_Q])
            done = true;

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
          continue;
        }

      if (redraw && al_is_event_queue_empty (queue))
        {
          al_clear_to_color (al_map_rgb (0, 0, 0));
          al_draw_filled_rectangle (obj.p.x, obj.p.y, obj.q.x, obj.q.y,
                                    obj.fill);
          al_draw_rectangle (obj.p.x, obj.p.y, obj.q.x, obj.q.y, obj.border,
                             obj.thck);

          //        al_draw_text (font, al_map_rgb (255, 255, 255), 10, 10,
          //                      ALLEGRO_ALIGN_LEFT, "pato sapato v0.1");

          al_draw_textf (font, al_map_rgb (255, 255, 255), 0, 0,
                         ALLEGRO_ALIGN_LEFT,
                         "p.x=%05.1f p.y=%05.1f q.x=%05.1f q.y=%05.1f",
                         obj.p.x, obj.p.y, obj.q.x, obj.q.y);

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
