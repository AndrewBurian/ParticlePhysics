/* ========================================================================
   $HEADER FILE
   $File: simulator.h $
   $Program: $
   $Developer: Jordan Marling $
   $Created On: 2015/11/18 $
   $Description: $
   $Revisions: $
   ======================================================================== */

#if !defined(SIMULATOR_H)
#define SIMULATOR_H

#include "render.h"
#include "universe.h"

enum {
	SIMULATION_NORMAL = 0x0,
	SIMULATION_PAN = 0x1,
	SIMULATION_UPDATEPARTICLE = 0x2,
	SIMULATION_UPDATEPARTICLE_VELOCITY = 0x3,
};
enum {
	HOTPARTICLE_VELOCITY = 0x0,
	HOTPARTICLE_MASS = 0x1,
	HOTPARTICLE_CHARGE = 0x2,

	HOTPARTICLE_COUNT,
};

struct simulation {

	int running;
	int paused;

	int state;
	int lastClickX;
	int lastClickY;

	int hotParticle;
	int hotParticleState;

	int savingTime;

};

struct renderstate;
struct universe;
void saveToFile(struct renderstate *, struct universe *);

#endif
