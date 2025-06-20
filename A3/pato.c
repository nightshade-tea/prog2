#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "lib/common.h"
#include "lib/game.h"

ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_DISPLAY *disp;
ALLEGRO_FONT *font;

static void
init_allegro ()
{
  ensure (al_init ());
  ensure (al_install_keyboard ());
  ensure (al_init_font_addon ());
  ensure (al_init_image_addon ());
  ensure (al_init_primitives_addon ());

  ensure (timer = al_create_timer (1.0 / FPS));
  ensure (queue = al_create_event_queue ());
  ensure (font = al_create_builtin_font ());
}

static void
init_display ()
{
  ALLEGRO_TRANSFORM transf;
  float scalex, scaley;

  al_set_new_display_flags (ALLEGRO_FULLSCREEN_WINDOW);
  ensure (disp = al_create_display (RENDER_WIDTH, RENDER_HEIGHT));

  scalex = ((float)al_get_display_width (disp)) / RENDER_WIDTH;
  scaley = ((float)al_get_display_height (disp)) / RENDER_HEIGHT;

  al_identity_transform (&transf);
  al_scale_transform (&transf, scalex, scaley);
  al_use_transform (&transf);

  al_hide_mouse_cursor (disp);
}

static void
init_queue ()
{
  al_register_event_source (queue, al_get_keyboard_event_source ());
  al_register_event_source (queue, al_get_display_event_source (disp));
  al_register_event_source (queue, al_get_timer_event_source (timer));
}

int
main ()
{
  init_allegro ();
  init_display ();
  init_queue ();

  start_game ();

  al_destroy_font (font);
  al_destroy_display (disp);
  al_destroy_timer (timer);
  al_destroy_event_queue (queue);

  return 0;
}
