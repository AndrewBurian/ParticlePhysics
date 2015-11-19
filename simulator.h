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

enum {
	SIMULATION_NORMAL = 0x0,
	SIMULATION_PAN = 0x1,
	SIMULATION_ADDPARTICLE = 0x2,
	SIMULATION_UPDATEPARTICLE = 0x3,
};

struct simulation {

	int running;
	int paused;

	int state;

	struct particle *hotParticle;
	uint last_click;
	int last_click_x;
	int last_click_y;

};

#endif
