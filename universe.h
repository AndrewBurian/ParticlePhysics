#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <stdio.h>

struct particle {
	int isActive;
	int isStationary;

	double xPos;
	double yPos;

	double xVel;
	double yVel;

	double xForce;
	double yForce;

	double mass;
	double charge;
	double size;
};

struct universe {
	int paused;

	struct particle* particles;
	int particleCount;
	int highestParticle;

};

struct universe* universeInit(int);
struct universe* universeInitFromFile(FILE*);
void universeExpand(struct universe*);
void addParticle(struct universe*, struct particle*);
void deleteParticle(struct universe*, struct particle*);
struct particle* getParticle(double, double);

#endif
