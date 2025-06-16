#include <stdlib.h>

#include "keyboard.h"

void
kbd_init (KEYBOARD key[ALLEGRO_KEY_MAX])
{
  memset (key, 0, ALLEGRO_KEY_MAX);
}

void
kbd_press (KEYBOARD key[ALLEGRO_KEY_MAX], int keycode)
{
  key[keycode] = KEY_SEEN | KEY_DOWN;
}

void
kbd_release (KEYBOARD key[ALLEGRO_KEY_MAX], int keycode)
{
  key[keycode] &= ~KEY_DOWN;
}

void
kbd_reset_seen (KEYBOARD key[ALLEGRO_KEY_MAX])
{
  for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
    key[i] &= ~KEY_SEEN;
}
