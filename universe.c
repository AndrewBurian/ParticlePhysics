#include "universe.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int addParticle(struct universe *univ, struct particle *p)
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
				if (i >= univ->nextParticle) {
					univ->nextParticle = i + 1;
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
	return i;
}

void deleteParticle(struct universe *univ, struct particle *p)
{

	int i = 0;

	// de-activate particle
	p->isActive = 0;

	// adjust highest particle

	// loop through the universe of active particles
	for (i = univ->nextParticle; i > 0; i--) {
		if (univ->particles[i].isActive) {
			break;
		}
	}

	univ->nextParticle = i + 1;

}

int getParticle(struct universe *univ, double xPos, double yPos)
{
	int i;
	struct particle *p;

	for (i = 0; i < univ->nextParticle; i++) {
		p = &univ->particles[i];
		if (sqrt(pow(xPos - p->xPos, 2) + pow(yPos - p->yPos, 2)) <=
		    p->size) {
			return i;
		}
	}

	return -1;
}

void universeExpand(struct universe *univ)
{

	int i = 0;
	int prev = univ->particleCount;

	// double particle space
	if (!univ->particleCount) {
		univ->particleCount = 1;
	} else {
		univ->particleCount *= 2;
	}

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
	univ->scale = 1000;
	univ->speed = 1;
	univ->fidelity = 1;
	univ->particleCount = size;
	univ->nextParticle = 0;

	univ->particles = calloc(size, sizeof(struct particle));

	for (i = 0; i < univ->particleCount; i++) {
		univ->particles[i].isActive = 0;
	}

	return univ;
}

int readFileLine(char **line, size_t * n, FILE * file)
{
	int count = 0;
	int i = 0;

	while (1) {
		count = getline(line, n, file);

		if (count == -1) {
			return count;
		}

		if (count == 0) {
			continue;
		}

		for (i = 0; i < count; i++) {
			if ((*line)[i] == '#' || (*line)[i] == '\n') {
				(*line)[i] = 0;
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

	memset(univ, 0, sizeof(struct universe));

	// read universe scale
	if (readFileLine(&line, &lineSize, file) == -1) {
		free(univ);
		free(line);
		return 0;
	}
	if (sscanf(line, "%lf", &univ->scale) != 1) {
		free(univ);
		free(line);
		return 0;
	}
	// read universe speed
	if (readFileLine(&line, &lineSize, file) == -1) {
		free(univ);
		free(line);
		return 0;
	}
	if (sscanf(line, "%lf", &univ->speed) != 1) {
		free(univ);
		free(line);
		return 0;
	}
	// universe fidelity
	if (readFileLine(&line, &lineSize, file) == -1) {
		free(univ);
		free(line);
		return 0;

	}
	if (sscanf(line, "%d", &univ->fidelity) != 1) {
		free(univ);
		free(line);
		return 0;
	}
	// read particles
	while (readFileLine(&line, &lineSize, file) != -1) {
		if (sscanf
		    (line, "%d %lf %lf %lf %lf %lf %lf %lf",
		     &p.isStationary, &p.xPos, &p.yPos, &p.xVel, &p.yVel,
		     &p.mass, &p.charge, &p.size) == 8) {
			addParticle(univ, &p);
		}
	}

	free(line);

	return univ;
}

void universeToFile(struct universe *univ, FILE * file)
{
	struct particle *p;
	int i;

	fprintf(file, "%lf\n%lf\n%d\n", univ->scale, univ->speed,
		univ->fidelity);

	for (i = 0; i < univ->nextParticle; i++) {
		if (univ->particles[i].isActive) {
			p = &univ->particles[i];
			fprintf(file, "%d %lf %lf %lf %lf %lf %lf %lf\n",
				p->isStationary, p->xPos, p->yPos, p->xVel,
				p->yVel, p->mass, p->charge, p->size);
		}
	}
}

void freeUniverse(struct universe *univ)
{

	free(univ->particles);
	free(univ);

}
