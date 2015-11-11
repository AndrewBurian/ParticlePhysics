#include "universe.h"
#include "physics.h"
#include "render.h"
#include <SDL2/SDL.h>

void simultateMode(struct universe*);
void addParticleMode(struct universe*);

int main(int argc, char **argv) {
	return 0;
}

void simulateMode(struct universe *univ) {

	int running = 1;
	SDL_Event ev;

	// main process loop
	while(running) {

		// loop through events
		while(SDL_PollEvent(&ev)) {

			switch(ev.type) {

			case SDL_MOUSEBUTTONDOWN:
				addParticleMode(univ);
				break;

			}
		}

		physicsApply(univ);

		renderUniverse(univ);

	}

}

void addParticleMode(struct universe *univ) {
	
	int addDone = 0;
	SDL_Event ev;

	// add process loop
	while(!addDone) {

		// loop through events
		while(SDL_PollEvent(&ev)) {

			switch(ev.type) {

			case SDL_MOUSEBUTTONDOWN:
				addParticleMode(univ);
				break;

			}
		}

		// render things somehow
	}
}
