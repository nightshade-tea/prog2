#ifndef MENU_H
#define MENU_H 1

#define MENU_CENTER_X RENDER_WIDTH / 2.0
#define MENU_CENTER_Y RENDER_HEIGHT / 2.0

#define MENU_DOT_UPDATE_RATE (int)(15 * (FPS / 30))

#define MENU_BACKGROUND_COLOR 29, 29, 29
#define MENU_FOREGROUND_COLOR 221, 221, 221
#define MENU_SELECT_COLOR 117, 29, 29
#define MENU_SHADOW_COLOR 74, 74, 74

#define MENU_TITLE "pato"
#define MENU_TITLE_X MENU_CENTER_X
#define MENU_TITLE_Y MENU_CENTER_Y - 50.0

#define MENU_DUCK_X MENU_CENTER_X - 23.0
#define MENU_DUCK_Y MENU_CENTER_Y - 20.0
#define MENU_DUCK_SCALE 2.0

#define MENU_START "iniciar "
#define MENU_START_DOT "iniciar."
#define MENU_START_X MENU_CENTER_X + 4.0
#define MENU_START_Y MENU_CENTER_Y + 50.0

#define MENU_EXIT "sair "
#define MENU_EXIT_DOT "sair."
#define MENU_EXIT_X MENU_CENTER_X + 4.0
#define MENU_EXIT_Y MENU_CENTER_Y + 70.0

/* 1 = start, 0 = exit */
unsigned char start_menu ();

#endif /* menu.h */
