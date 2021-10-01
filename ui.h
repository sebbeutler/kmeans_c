#ifndef UI_H
#define UI_H

#include <SDL.h>

#include "data_structures/clist.h"

#define UI_WIDTH 900
#define UI_HEIGHT 500

extern SDL_Window* ui_screen;
extern SDL_Renderer* ui_renderer;

void ui_init();

void ui_run(bool (*draw)(), void (*event)(SDL_Event* e));

void ui_exit();

void DrawCircle(SDL_Renderer *renderer, int x, int y, int radius);

#endif // !UI_H