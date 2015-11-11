#ifndef UNIVERSE_H
#define UNIVERSE_H

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
};

#endif
