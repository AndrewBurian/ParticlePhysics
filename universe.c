#include "universe.h"
#include <string.h>
#include <stdlib.h>

void addParticle(struct universe *univ, struct particle *p) {

	int i = 0;

	// continue looping until particle is added
	while(1) {

		// loop through the universe
		for (i = 0; i < univ->particleCount; i++) {

			// find an unused particle
			if(!univ->particles[i].isActive) {

				// copy the particle into the universe
				memcpy(&univ->particles[i], p, sizeof(struct particle));
				univ->particles[i].isActive = 1;

				// adjust highest particle if needed
				if(i > univ->highestParticle) {
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

void deleteParticle(struct universe *univ, struct particle *p) {

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

struct particle* getParticle(double xPos, double yPos) {
	return 0;
}

void universeExpand(struct universe *univ) {

	int prev = univ->particleCount;

	// double particle space
	univ->particleCount *= 2;

	univ->particles = realloc(univ->particles, univ->particleCount * sizeof(struct particle));

	// zero out new particle space
	memset(&univ->particles[prev], 0, (univ->particleCount - prev) * sizeof(struct particle));

}
