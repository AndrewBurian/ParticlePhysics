#include "universe.h"
#include "physics.h"
#include "render.h"
#include "simulator.h"
#include "input.h"
#include <SDL2/SDL.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

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
	// create universe and renderer
	if (!universeFile) {

		// new universe
		univ = universeInit(10);

		// new renderstate
		render = renderstateInit("Particle Simulator", 640, 480);

	} else {
		render = renderstateInitFromFile(universeFile);
		univ = universeInitFromFile(universeFile);

		if (render == 0) {
			fprintf(stderr, "Error loading rendering struct.\n");
		}
		if (univ == 0) {
			fprintf(stderr, "Error loading universe struct.\n");
		}
		fclose(universeFile);
	}

	// check for errors
	if (!univ) {
		fprintf(stderr, "Failed to initialize universe\n");
		return -3;

	}

	if (!render) {
		fprintf(stderr, "Failed to initialize the renderer\n");
		return -4;

	}
	// Create the simulation state
	sim = simulationInit();

	// run simulation
	simulateMode(render, univ, sim);

	// finished
	freeUniverse(univ);
	freeRenderstate(render);

	return 0;
}

void
simulateMode(struct renderstate *render, struct universe *univ,
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
	sim->paused = 1;

	sim->state = SIMULATION_NORMAL;

	sim->hotParticle = -1;
	sim->lastClickX = -1;
	sim->lastClickX = -1;

	sim->savingTime = -1;

	return sim;
}

void saveToFile(struct renderstate *render, struct universe *univ)
{

	char fileName[20] = { 0 };
	FILE *file = 0;
	int fileCount;

	for (fileCount = 1; fileCount < 999; fileCount++) {
		sprintf(fileName, "universe-%d.save", fileCount);
		if (access(fileName, F_OK) == -1) {
			// file does not exist
			break;
		}
	}

	file = fopen(fileName, "w");

	renderstateToFile(render, file);
	universeToFile(univ, file);

	fclose(file);
}
