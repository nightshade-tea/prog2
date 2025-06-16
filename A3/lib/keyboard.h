#ifndef KEYBOARD_H
#define KEYBOARD_H 1

#include <allegro5/allegro5.h>

#define KEY_SEEN 1
#define KEY_DOWN 2

typedef unsigned char KEYBOARD;

void kbd_init (KEYBOARD key[ALLEGRO_KEY_MAX]);

void kbd_press (KEYBOARD key[ALLEGRO_KEY_MAX], int keycode);

void kbd_release (KEYBOARD key[ALLEGRO_KEY_MAX], int keycode);

void kbd_reset_seen (KEYBOARD key[ALLEGRO_KEY_MAX]);

#endif /* keyboard.h */
