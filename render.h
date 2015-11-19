#ifndef RENDER_H
#define RENDER_H

#include "universe.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct renderstate {

	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font_small;
	TTF_Font *font_large;

	double xPos;
	double yPos;

	float scale;
	int width;
	int height;

};

struct renderstate *renderCreate(const char *title, int width, int height);
void renderUniverse(struct renderstate *state, struct universe *);

#endif
