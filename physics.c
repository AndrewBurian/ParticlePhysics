#include "physics.h"
#include "universe.h"
#include <math.h>

void applyForcePart(const double constant, struct particle*, struct particle*);

void physicsApply(struct universe *univ) {
	applyGravity(univ);
	applyElectric(univ);
	applyMovement(univ);
	applyCollision(univ);
}

void applyGravity(struct universe* univ) {
	int i, j = 0;

	for (i = 0; i < univ->highestParticle; i++) {
		if (!univ->particles[i].isActive) {
			continue;
		}

		for (j = i; j < univ->highestParticle; j++) {
			if(!univ->particles[j].isActive) {
				continue;
			}

			applyForcePart(GRAVITIATION, &univ->particles[i], &univ->particles[j]);

		}

	}
}

void applyElectric(struct universe* univ) {

	int i, j = 0;

	for (i = 0; i < univ->highestParticle; i++) {
		if (!univ->particles[i].isActive) {
			continue;
		}

		for (j = i; j < univ->highestParticle; j++) {
			if(!univ->particles[j].isActive) {
				continue;
			}

			applyForcePart(ELECTROSTATIC, &univ->particles[i], &univ->particles[j]);

		}

	}
}

void applyForcePart(const double constant, struct particle* a, struct particle* b) {

	double xDist = a->xPos - b->xPos;
	double yDist = a->yPos - b->yPos;
	double distance = sqrt(pow(xDist, 2) + pow(yDist, 2));

	double totalForce = constant * ((a->mass * b->mass) / pow(distance, 2));

	a->xForce -= totalForce * xDist / distance;
	a->yForce -= totalForce * yDist / distance;

	b->xForce += totalForce * xDist / distance;
	b->yForce += totalForce * yDist / distance;

}

void applyMovement(struct universe* univ) {

}

void applyCollision(struct universe* univ) {

}
