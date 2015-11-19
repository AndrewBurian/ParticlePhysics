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

#include "render.h"
#include "simulator.h"
#include "universe.h"

void handleInput(struct simulation *sim, struct universe *univ,
		 struct renderstate *render)
{

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			sim->running = 0;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				sim->running = 0;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			if (event.button.button == SDL_BUTTON_LEFT) {
				sim->state = SIMULATION_PAN;
			}

			break;

		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {
				sim->state = SIMULATION_NORMAL;
			}

			break;

		case SDL_MOUSEMOTION:

			if (sim->state == SIMULATION_PAN) {

				render->xPos += event.motion.xrel;
				render->yPos += event.motion.yrel;

			}

			break;

		case SDL_MOUSEWHEEL:

			if (event.wheel.x + event.wheel.y < 0) {
				render->scale /= 1.1;
			} else {
				render->scale *= 1.1;
			}

			break;

		}
	}

}
