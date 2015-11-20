#ifndef PHYSICS_H
#define PHYSICS_H

#include "universe.h"

#define GRAVITIATION 6.77e-11L
#define ELECTROSTATIC 8.987e+9L
#define DENSITY 1000		// kg/m^2

void physicsApply (struct universe *);
void applyGravity (struct universe *);
void applyElectric (struct universe *);
void applyMovement (struct universe *);
void applyCollision (struct universe *);
void setParticleSize (struct particle *);

#endif
