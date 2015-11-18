#include "universe.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void addParticle(struct universe *univ, struct particle *p)
{

	int i = 0;

	// continue looping until particle is added
	while (1) {

		// loop through the universe
		for (i = 0; i < univ->particleCount; i++) {

			// find an unused particle
			if (!univ->particles[i].isActive) {

				// copy the particle into the universe
				memcpy(&univ->particles[i], p,
				       sizeof(struct particle));
				univ->particles[i].isActive = 1;

				// adjust highest particle if needed
				if (i > univ->highestParticle) {
					univ->highestParticle = i;
				}
				// particle added
				break;
			}
		}

		// if the particle was added
		if (i < univ->particleCount) {
			break;
		}
		// otherwise expand the universe and try again
		universeExpand(univ);
	}
}

void deleteParticle(struct universe *univ, struct particle *p)
{

	int i = 0;

	// de-activate particle
	p->isActive = 0;

	// adjust highest particle

	// loop through the universe of active particles
	for (i = univ->highestParticle; i > 0; i--) {
		if (univ->particles[i].isActive) {
			break;
		}
	}

	univ->highestParticle = i;

}

struct particle *getParticle(double xPos, double yPos)
{
	return 0;
}

void universeExpand(struct universe *univ)
{

	int i = 0;
	int prev = univ->particleCount;

	// double particle space
	univ->particleCount *= 2;

	univ->particles =
	    realloc(univ->particles,
		    univ->particleCount * sizeof(struct particle));

	// zero out new particle space
	for (i = prev; i < univ->particleCount; i++) {
		univ->particles[i].isActive = 0;
	}

}

struct universe *universeInit(int size)
{

	int i = 0;

	struct universe *univ = malloc(sizeof(struct universe));
	univ->paused = 1;
	univ->scale = 0.1;
	univ->speed = 1;
	univ->particleCount = size;
	univ->highestParticle = 0;

	univ->particles = calloc(size, sizeof(struct particle));

	for (i = 0; i < univ->particleCount; i++) {
		univ->particles[i].isActive = 0;
	}

	return univ;
}

int readFileLine(char *line, size_t * n, FILE * file)
{
	int count = 0;
	int i = 0;

	while (1) {
		count = getline(&line, n, file);

		if (count == -1) {
			return count;
		}

		if (count == 0) {
			continue;
		}

		for (i = 0; i < count; i++) {
			if (line[i] == '#') {
				line[i] = 0;
				break;
			}
		}

		if (i >= 1) {
			return i;
		}
	}
}

struct universe *universeInitFromFile(FILE * file)
{
	struct universe *univ = malloc(sizeof(struct universe));
	char *line = 0;
	size_t lineSize = 0;

	struct particle p = { 0 };

	// read universe scale
	if (readFileLine(line, &lineSize, file) == -1) {
		free(univ);
		return 0;

	}

	if (sscanf(line, "%lf", &univ->scale) != 1) {
		free(univ);
		return 0;
	}
	// read universe speed
	if (readFileLine(line, &lineSize, file) == -1) {
		free(univ);
		return 0;
	}

	if (sscanf(line, "%lf", &univ->speed) != 1) {
		free(univ);
		return 0;
	}
	// read particles
	while (readFileLine(line, &lineSize, file) != -1) {
		if (sscanf
		    (line, "%d %d %lf %lf %lf %lf %lf %lf %lf", &p.isActive,
		     &p.isStationary, &p.xPos, &p.yPos, &p.xVel, &p.yVel,
		     &p.mass, &p.charge, &p.size) == 9) {
			addParticle(univ, &p);
		}
	}

	return univ;
}

void saveToFile(struct universe *univ)
{

}

void freeUniverse(struct universe *univ)
{

	free(univ->particles);
	free(univ);

}
