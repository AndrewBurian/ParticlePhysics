#include "universe.h"
#include <string.h>

void addParticle(struct universe *univ, struct particle *p) {

	int i = 0;

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
		}
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
