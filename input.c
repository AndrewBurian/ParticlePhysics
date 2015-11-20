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

#include "physics.h"
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
			} else if (event.key.keysym.scancode ==
				   SDL_SCANCODE_SPACE) {
				if (sim->paused) {
					sim->state = SIMULATION_NORMAL;
					sim->hotParticle = -1;
					sim->paused = 0;
				} else {
					sim->paused = 1;
				}
			} else if (event.key.keysym.scancode ==
				   SDL_SCANCODE_LEFT) {
				render->xPos += 5 / render->scale;
			} else if (event.key.keysym.scancode ==
				   SDL_SCANCODE_RIGHT) {
				render->xPos -= 5 / render->scale;
			} else if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				render->yPos += 5 / render->scale;
			} else if (event.key.keysym.scancode ==
				   SDL_SCANCODE_DOWN) {
				render->yPos -= 5 / render->scale;
			} else if (event.key.keysym.scancode ==
				   SDL_SCANCODE_KP_PLUS
				   || (event.key.keysym.scancode ==
				       SDL_SCANCODE_EQUALS
				       && (event.key.keysym.
					   mod & KMOD_SHIFT))) {
				univ->speed *= 1.1f;
			} else if (event.key.keysym.scancode ==
				   SDL_SCANCODE_KP_MINUS
				   || event.key.keysym.scancode ==
				   SDL_SCANCODE_MINUS) {
				univ->speed /= 1.1f;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			sim->last_click_x = event.button.x;
			sim->last_click_y = event.button.y;

			if (event.button.button == SDL_BUTTON_LEFT) {
				if (sim->state == SIMULATION_NORMAL) {

					double x =
					    ((event.button.x -
					      (render->width / 2)) /
					     render->scale) - render->xPos;
					double y =
					    ((event.button.y -
					      (render->height / 2)) /
					     render->scale) - render->yPos;

					// Convert from pixels to meters
					x /= univ->scale;
					y /= univ->scale;

					int p = getParticle(univ, x, y);
					if (p != -1) {
						sim->paused = 1;
						sim->hotParticle = p;
						sim->state =
						    SIMULATION_UPDATEPARTICLE;
						sim->hotParticleState =
						    HOTPARTICLE_VELOCITY;
					} else {
						sim->state = SIMULATION_PAN;
					}
				} else if (sim->state ==
					   SIMULATION_UPDATEPARTICLE) {
					if (sim->hotParticleState ==
					    HOTPARTICLE_VELOCITY) {
						sim->state =
						    SIMULATION_UPDATEPARTICLE_VELOCITY;
					}
				}
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				if (sim->state & SIMULATION_UPDATEPARTICLE) {
					sim->hotParticleState++;
					if (sim->hotParticleState >=
					    HOTPARTICLE_COUNT) {
						sim->hotParticleState = 0;
					}
				}
			}

			break;

		case SDL_MOUSEBUTTONUP:

			if (event.button.button == SDL_BUTTON_LEFT) {

				if (sim->state ==
				    SIMULATION_UPDATEPARTICLE_VELOCITY) {
					sim->state = SIMULATION_UPDATEPARTICLE;
				} else if (sim->state !=
					   SIMULATION_UPDATEPARTICLE) {
					if (sim->last_click_x == event.button.x
					    && sim->last_click_y ==
					    event.button.y) {
						struct particle p;

						p.isActive = 1;
						p.isStationary = 0;
						p.xPos =
						    ((event.button.x -
						      (render->width / 2)) /
						     render->scale) -
						    render->xPos;
						p.yPos =
						    ((event.button.y -
						      (render->height / 2)) /
						     render->scale) -
						    render->yPos;
						p.xVel = 0;
						p.yVel = 0;
						p.xForce = 0;
						p.yForce = 0;
						p.mass =
						    1 * (1 / render->scale);
						p.charge = 0;

						// Convert from pixels to meters
						p.xPos /= univ->scale;
						p.yPos /= univ->scale;

						setParticleSize(&p);

						sim->hotParticle =
						    addParticle(univ, &p);
						sim->hotParticleState =
						    HOTPARTICLE_VELOCITY;

						sim->state =
						    SIMULATION_UPDATEPARTICLE;
						sim->paused = 1;

					} else {
						sim->state = SIMULATION_NORMAL;
					}
				}
			}

			break;

		case SDL_MOUSEMOTION:

			if (sim->state == SIMULATION_PAN) {

				render->xPos +=
				    event.motion.xrel / render->scale;
				render->yPos +=
				    event.motion.yrel / render->scale;

			} else if (sim->state ==
				   SIMULATION_UPDATEPARTICLE_VELOCITY) {

				struct particle *p =
				    &univ->particles[sim->hotParticle];

				// Convert the particle position into pixels
				double x =
				    (((p->xPos * univ->scale) +
				      render->xPos) * render->scale) +
				    (render->width / 2);
				double y =
				    (((p->yPos * univ->scale) +
				      render->yPos) * render->scale) +
				    (render->height / 2);

				double xDist = event.motion.x - x;
				double yDist = event.motion.y - y;

				// Convert the distances to m/s
				xDist /= render->scale * univ->scale;
				yDist /= render->scale * univ->scale;

				p->xVel = xDist / 10;
				p->yVel = yDist / 10;
			}

			break;

		case SDL_MOUSEWHEEL:

			if (sim->state == SIMULATION_UPDATEPARTICLE
			    && sim->hotParticleState != HOTPARTICLE_VELOCITY) {
				if (sim->hotParticleState == HOTPARTICLE_MASS) {
					struct particle *p =
					    &univ->particles[sim->hotParticle];

					if (event.wheel.x + event.wheel.y < 0) {
						p->mass /= 1.1;
					} else {
						p->mass *= 1.1;
					}

					setParticleSize(p);
				} else if (sim->hotParticleState ==
					   HOTPARTICLE_CHARGE) {
					struct particle *p =
					    &univ->particles[sim->hotParticle];

					if (event.wheel.x + event.wheel.y < 0) {
						p->charge -= 0.1;
					} else {
						p->charge += 0.1;
					}

				}
			} else {
				if (event.wheel.x + event.wheel.y < 0) {
					render->scale /= 1.1;
				} else {
					render->scale *= 1.1;
				}
			}

			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
			    || event.window.event == SDL_WINDOWEVENT_RESIZED) {
				render->width = event.window.data1;
				render->height = event.window.data2;
			}
			break;

		}
	}

}
