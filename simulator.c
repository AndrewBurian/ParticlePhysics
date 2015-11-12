#include "universe.h"
#include "physics.h"
#include "render.h"
#include <SDL2/SDL.h>
#include <getopt.h>
#include <stdio.h>

void simulateMode(struct universe*);
void addParticleMode(struct universe*);

int main(int argc, char **argv) {

	// args setup
	char *shortOpts = "f:";
	struct option longOpts[] = {
		{"file",	required_argument,	0,	'f'},
		{0,	0,	0,	0}
	};
	char c = 0;
	int optionIndex = 0;

	// file for loading universe
	char *universeFilePath = 0;
	FILE *universeFile = 0;

	// universe
	struct universe *univ = 0;

	// process arguments
	while ((c = getopt_long(argc, argv, shortOpts, longOpts, &optionIndex)) != -1) {
		switch(c) {

		case 'f':
			universeFilePath = optarg;
			break;

		default:
			return -1;

		}
	}

	// attempt to open file
	if (universeFilePath) {
		universeFile = fopen(universeFilePath, "r");
		if (!universeFile) {
			fprintf(stderr, "Failed to open file %s\n", universeFilePath);
			return -2;
		}
	}

	// load universe
	if (universeFile) {
		univ = universeInitFromFile(universeFile);
		fclose(universeFile);
	} else {
		univ = universeInit(10);
	}

	if (!univ) {
		fprintf(stderr, "Failed to initialize universe\n");
		return -3;
	}

	// run simulation
	simulateMode(univ);

	// finished
	freeUniverse(univ);

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
