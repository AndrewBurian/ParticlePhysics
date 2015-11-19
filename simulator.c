#include "universe.h"
#include "physics.h"
#include "render.h"
#include "simulator.h"
#include "input.h"
#include <SDL2/SDL.h>
#include <getopt.h>
#include <stdio.h>

struct simulation *simulationInit();
void simulateMode(struct renderstate *, struct universe *, struct simulation *);

int main(int argc, char **argv)
{

	// args setup
	const char *shortOpts = "f:";
	struct option longOpts[] = {
		{"file", required_argument, 0, 'f'},
		{0, 0, 0, 0}
	};
	char c = 0;
	int optionIndex = 0;

	// file for loading universe
	char *universeFilePath = 0;
	FILE *universeFile = 0;

	// state information
	struct universe *univ = 0;
	struct renderstate *render = 0;
	struct simulation *sim = 0;

	// process arguments
	while ((c =
		getopt_long(argc, argv, shortOpts, longOpts,
			    &optionIndex)) != -1) {
		switch (c) {

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
			fprintf(stderr, "Failed to open file %s\n",
				universeFilePath);
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
	// create the renderstate
	render = renderCreate("Particle Simulator", 640, 480);
	if (render == 0) {
		fprintf(stderr, "Failed to initialize the renderer\n");
		return -4;
	}
	// Create the simulation state
	sim = simulationInit();

	// run simulation
	simulateMode(render, univ, sim);

	// finished
	freeUniverse(univ);

	return 0;
}

void simulateMode(struct renderstate *render, struct universe *univ,
		  struct simulation *sim)
{

	sim->running = 1;

	// main process loop
	while (sim->running) {

		handleInput(sim, univ, render);

		if (sim->paused == 0) {
			physicsApply(univ);
		}

		renderUniverse(render, sim, univ);

	}
}

struct simulation *simulationInit()
{
	struct simulation *sim =
	    (struct simulation *)malloc(sizeof(struct simulation));

	sim->running = 0;
	sim->paused = 0;

	sim->state = SIMULATION_NORMAL;

	sim->hotParticle = -1;
	sim->last_click_x = -1;
	sim->last_click_y = -1;

	return sim;
}
