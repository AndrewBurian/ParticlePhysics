#include "physics.h"
#include "universe.h"
#include <math.h>

void applyForcePart(const double constant, struct particle*, struct particle*);
int collideParticles(struct particle*, struct particle*);

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

	int i = 0;
	struct particle *p;

	double xAccel, yAccel;

	for(i = 0; i < univ->highestParticle; ++i){

		p = &univ->particles[i];

		if(!p->isActive || p->isStationary) {
			continue;
		}

		// F = ma
		// a = F / m
		xAccel = (p->xForce / p->mass);
		yAccel = (p->yForce / p->mass);

		// update position
		// x = v t + 0.5 a t^2
		p->xPos += (p->xVel * univ->speed) + ( 0.5 * xAccel * (pow(univ->speed, 2)));
		p->yPos += (p->yVel * univ->speed) + ( 0.5 * yAccel * (pow(univ->speed, 2)));

		// update velocities
		p->xVel += xAccel * univ->speed;
		p->yVel += yAccel * univ->speed;

		p->xForce = 0;
		p->yForce = 0;
	}
}

void applyCollision(struct universe* univ) {

	int i, j = 0;

	for (i = 0; i < univ->highestParticle; i++) {
		if (!univ->particles[i].isActive) {
			continue;
		}

		for (j = i; j < univ->highestParticle; j++) {
			if(!univ->particles[j].isActive) {
				continue;
			}

			if (collideParticles(&univ->particles[i], &univ->particles[j])) {
				deleteParticle(univ, &univ->particles[j]);
			}

		}

	}
}

int collideParticles(struct particle* a, struct particle* b) {
	double distance = sqrt(pow(a->xPos - b->xPos, 2) + pow(a->yPos - b->yPos, 2));

	// no collision check
	if (distance > a->size + b->size) {
		return 0;
	}

	// combine into particle a

	// add charge
	a->charge += b->charge;

	// mass-weighted new position
	a->xPos = ((a->xPos * a->mass) + (b->xPos * b->mass)) / (a->mass + b->mass);
	a->xPos = ((a->yPos * a->mass) + (b->yPos * b->mass)) / (a->mass + b->mass);

	// conservation of momentum
	a->xVel = ((a->xVel * a->mass) + (b->xVel * b->mass)) / (a->mass + b->mass);
	a->yVel = ((a->yVel * a->mass) + (b->yVel * b->mass)) / (a->mass + b->mass);

	// consertation of mass
	a->mass += b->mass;

	// stationary particles remain so
	a->isStationary += b->isStationary;

	// size is a function of density and mass
	a->size = sqrt(a->mass / DENSITY / M_PI);

	return 1;
}
