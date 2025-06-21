#ifndef GAME_H
#define GAME_H 1

#define DBG 1

#define RENDER_WIDTH 576
#define RENDER_HEIGHT 324

#define FPS 60.0
#define GRAV 2.0 / (FPS / 30)

#define PAUSE_DIM_COLOR 29, 29, 29, 157

#define PAUSE_MSG "aperte ESC para continuar ou Q para sair"
#define PAUSE_MSG_X RENDER_WIDTH / 2.0
#define PAUSE_MSG_Y RENDER_HEIGHT / 4.0
#define PAUSE_MSG_FOREGROUND_COLOR 221, 221, 221
#define PAUSE_MSG_SHADOW_COLOR 74, 74, 74

void start_game ();

#endif /* game.h */
