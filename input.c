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

			sim->last_click = event.button.timestamp;
			sim->last_click_x = event.button.x;
			sim->last_click_y = event.button.y;

			if (sim->state == SIMULATION_NORMAL
			    && event.button.button == SDL_BUTTON_LEFT) {
				sim->state = SIMULATION_PAN;
			}

			break;

		case SDL_MOUSEBUTTONUP:

			if (sim->state != SIMULATION_ADDPARTICLE) {

				if (event.button.timestamp - sim->last_click <
				    100 && sim->last_click_x == event.button.x
				    && sim->last_click_y == event.button.y) {
					struct particle p;

					p.isActive = 1;
					p.isStationary = 0;
					p.xPos =
					    render->xPos + event.button.x -
					    (render->width / 2);
					p.yPos =
					    render->yPos + event.button.y -
					    (render->height / 2);
					p.xVel = 0;
					p.yVel = 0;
					p.xForce = 0;
					p.yForce = 0;
					p.mass = 10 * render->scale;
					p.charge = 0;
					p.size = 10 * render->scale;

					addParticle(univ, &p);

					sim->state = SIMULATION_ADDPARTICLE;
				} else if (event.button.button ==
					   SDL_BUTTON_LEFT) {
					sim->state = SIMULATION_NORMAL;
				}
			}

			break;

		case SDL_MOUSEMOTION:

			if (sim->state == SIMULATION_PAN) {

				render->xPos +=
				    event.motion.xrel / render->scale;
				render->yPos +=
				    event.motion.yrel / render->scale;

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
