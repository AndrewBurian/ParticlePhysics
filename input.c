/* ========================================================================
   $SOURCE FILE
   $File: input.cpp $
   $Program: $
   $Developer: Jordan Marling $
   $Created On: 2015/11/18 $
   $Functions: $
   $Description: $
   $Revisions: $
   ======================================================================== */

#include "input.h"

#include <SDL2/SDL.h>

#include "simulator.h"
#include "universe.h"

void handleInput(struct simulation *state, struct universe *univ)
{

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			state->running = 0;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				state->running = 0;
			}
			break;

		}
	}

}
