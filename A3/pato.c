#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "lib/common.h"
#include "lib/keyboard.h"

#define FPS 60.0
#define RENDER_WIDTH 320
#define RENDER_HEIGHT 200

/* (x1, y1) ----------+
 * |                  |
 * |                  |
 * +-----------(x2, y2) */
typedef struct OBJECT
{
  float x1, y1;         // upper left point
  float x2, y2;         // lower right point
  float xsz, ysz;       // x2 - x1, y2 - y1
  float vx, vy;         // movement speed vector
  float thck;           // border thickness
  ALLEGRO_COLOR fill;   // fill color
  ALLEGRO_COLOR border; // border color
} OBJECT;

void
obj_init (OBJECT *obj)
{
  obj->xsz = obj->ysz = (RENDER_WIDTH / RENDER_HEIGHT) * 10;

  obj->x1 = (RENDER_WIDTH - obj->xsz) / 2;
  obj->y1 = (RENDER_HEIGHT - obj->ysz) / 2;

  obj->x2 = obj->x1 + obj->xsz;
  obj->y2 = obj->y1 + obj->ysz;

  obj->vx = obj->vy = (obj->xsz / 4) / (FPS / 30);

  obj->thck = 1;
  obj->fill = al_map_rgb (0, 0, 255);
  obj->border = al_map_rgb (255, 0, 0);
}

void
obj_update_position (OBJECT *obj, KEYBOARD key[ALLEGRO_KEY_MAX])
{
  if (key[ALLEGRO_KEY_UP])
    {
      obj->y1 -= obj->vy;
      obj->y2 -= obj->vy;
    }

  if (key[ALLEGRO_KEY_DOWN])
    {
      obj->y1 += obj->vy;
      obj->y2 += obj->vy;
    }

  if (key[ALLEGRO_KEY_LEFT])
    {
      obj->x1 -= obj->vx;
      obj->x2 -= obj->vx;
    }

  if (key[ALLEGRO_KEY_RIGHT])
    {
      obj->x1 += obj->vx;
      obj->x2 += obj->vx;
    }
}

void
obj_keep_inside_bounds (OBJECT *obj)
{
  if (obj->x1 < 0.5)
    {
      obj->x1 = 0.5;
      obj->x2 = obj->x1 + obj->xsz;
    }

  else if (obj->x2 > RENDER_WIDTH - 0.5)
    {
      obj->x2 = RENDER_WIDTH - 0.5;
      obj->x1 = obj->x2 - obj->xsz;
    }

  if (obj->y1 < 0.5)
    {
      obj->y1 = 0.5;
      obj->y2 = obj->y1 + obj->ysz;
    }

  else if (obj->y2 > RENDER_HEIGHT - 0.5)
    {
      obj->y2 = RENDER_HEIGHT - 0.5;
      obj->y1 = obj->y2 - obj->ysz;
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

          if (key[ALLEGRO_KEY_ESCAPE])
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
          al_draw_filled_rectangle (obj.x1, obj.y1, obj.x2, obj.y2, obj.fill);
          al_draw_rectangle (obj.x1, obj.y1, obj.x2, obj.y2, obj.border,
                             obj.thck);

          //        al_draw_text (font, al_map_rgb (255, 255, 255), 10, 10,
          //                      ALLEGRO_ALIGN_LEFT, "pato sapato v0.1");

          al_draw_textf (font, al_map_rgb (255, 255, 255), 0, 0,
                         ALLEGRO_ALIGN_LEFT,
                         "(x1=%.1f y1=%.1f) (x2=%.1f y2=%.1f)", obj.x1, obj.y1,
                         obj.x2, obj.y2);

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
