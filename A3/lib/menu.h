#ifndef MENU_H
#define MENU_H 1

#define CENTER_X RENDER_WIDTH / 2.0
#define CENTER_Y RENDER_HEIGHT / 2.0

#define DOT_UPDATE_RATE (int)(15 * (FPS / 30))

#define BACKGROUND_COLOR 29, 29, 29
#define FOREGROUND_COLOR 221, 221, 221
#define SELECT_COLOR 117, 29, 29
#define SHADOW_COLOR 74, 74, 74

#define TITLE "pato"
#define TITLE_X CENTER_X
#define TITLE_Y CENTER_Y - 50.0

#define DUCK_X CENTER_X - 23.0
#define DUCK_Y CENTER_Y - 20.0
#define DUCK_SCALE 2.0

#define START "iniciar "
#define START_DOT "iniciar."
#define START_X CENTER_X + 4.0
#define START_Y CENTER_Y + 50.0

#define EXIT "sair "
#define EXIT_DOT "sair."
#define EXIT_X CENTER_X + 4.0
#define EXIT_Y CENTER_Y + 70.0

/* 1 = start, 0 = exit */
unsigned char start_menu ();

#endif /* menu.h */
