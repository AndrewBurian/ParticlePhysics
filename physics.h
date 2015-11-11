#ifndef PHYSICS_H
#define PHYSICS_H

#include "universe.h"

const double GRAVITIATION = 6.77e-11L;
const double ELECTROSTATIC = 8.987e+9L;

void physicsApply(struct universe*);
void applyGravity(struct universe*);
void applyElectric(struct universe*);
void applyMovement(struct universe*);
void applyCollision(struct universe*);


#endif
