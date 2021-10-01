#include "ui.h"

#include <SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "pcg_basic.h"

SDL_Window* ui_screen;
SDL_Renderer* ui_renderer;

void ui_init()
{
	// Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
       exit(1);
    }
    
    ui_screen = SDL_CreateWindow("kmeans",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            UI_WIDTH, UI_HEIGHT,
            0);
    if(!ui_screen) {
        fprintf(stderr, "Could not create window\n");
        exit(1);
    }
    ui_renderer = SDL_CreateRenderer(ui_screen, -1, SDL_RENDERER_SOFTWARE);
    if(!ui_renderer) {
        fprintf(stderr, "Could not create renderer\n");
        exit(1);
    }
}

void ui_run(bool (*draw)(), void (*event)(SDL_Event*))
{
	bool run = true;
	while (run)
	{
		SDL_Delay( 10 );
		#pragma warning ( disable : 4090 )
		unsigned char* keys = SDL_GetKeyboardState( NULL );

		SDL_Event e;
		// For mouse rectangle (static to presist between function calls)
		static int mx = -1, my = -1;

		// Clear the window to white
		SDL_SetRenderDrawColor( ui_renderer, 56, 138, 214, 255 );
		SDL_RenderClear( ui_renderer );

		// Event loop
		while ( SDL_PollEvent( &e ) != 0 ) {
			event(&e);
			switch ( e.type ) {
				case SDL_QUIT:
					run = false;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_MOUSEMOTION:
					mx = e.button.x;
					my = e.button.y;
				case SDL_MOUSEBUTTONUP:
					break;
			}
		}
		
		run = draw();

		// if ( keys[SDL_SCANCODE_SPACE] ) {
		//     SDL_Rect r = { 100, 100, mx-100, my-100 };
		// 	SDL_RenderDrawPoint( ui_renderer, 10, 10 );
		// 	SDL_RenderDrawLine( ui_renderer, 10, 20, 10, 100 );
		// 	SDL_RenderFillRect( ui_renderer, &r );
		// 	SDL_RenderDrawRect( ui_renderer, &r );
		// }

		if ( keys[SDL_SCANCODE_ESCAPE])
			run = false;

		// Update window
		SDL_RenderPresent( ui_renderer );
	}
	
	ui_exit();
}

void ui_exit()
{
	SDL_DestroyWindow(ui_screen);
    SDL_Quit();
}

void DrawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}