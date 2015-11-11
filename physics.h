#ifndef PHYSICS_H
#define PHYSICS_H

#include "universe.h"

void physicsApply(struct universe*);
void addParticle(struct particle*);
void deleteParticle(struct particle*);
struct particle* getParticle(double, double);

#endif
