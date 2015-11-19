#ifndef RENDER_H
#define RENDER_H

#include "universe.h"

#include <SDL2/SDL.h>

struct renderstate {

	SDL_Window *window;
	SDL_Renderer *renderer;

	float scale;

	int xPos;
	int yPos;
	int width;
	int height;

};

struct renderstate *renderCreate(const char *title, int width, int height);
void renderUniverse(struct renderstate *state, struct universe *);

#endif
