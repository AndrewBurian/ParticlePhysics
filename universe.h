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
	double scale;		// px/m
	double speed;		// s/tick
	int fidelity;

	struct particle *particles;
	int particleCount;
	int nextParticle;
};

struct universe *universeInit(int);
struct universe *universeInitFromFile(FILE *);
void universeExpand(struct universe *);
int addParticle(struct universe *, struct particle *);
void deleteParticle(struct universe *, struct particle *);
int getParticle(struct universe *, double, double);
void universeToFile(struct universe *, FILE *);
void freeUniverse(struct universe *);

int readFileLine(char **, size_t *, FILE *);

#endif
