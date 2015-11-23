#ifndef RENDER_H
#define RENDER_H

#include "simulator.h"
#include "universe.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct renderstate {

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Point *points;
	int point_count;

	TTF_Font *font_small;
	TTF_Font *font_large;

	double xPos;
	double yPos;

	float scale;
	int width;
	int height;

	int frames;
	int fps;
	unsigned int last_frame;

};

enum font_flags {
	FONT_LEFT = 0x1,
	FONT_RIGHT = 0x2,
	FONT_CENTER = 0x3,
};

struct renderstate *renderstateInit(const char *title, int width, int height);
struct renderstate *renderstateInitFromFile(FILE *);
void renderstateToFile(struct renderstate *, FILE *);
void freeRenderstate(struct renderstate *);
void renderUniverse(struct renderstate *, struct simulation *,
		    struct universe *);

#endif
