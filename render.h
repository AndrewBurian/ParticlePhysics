#ifndef RENDER_H
#define RENDER_H

#include "simulator.h"
#include "universe.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct renderstate
{

  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font_small;
  TTF_Font *font_large;

  double xPos;
  double yPos;

  float scale;
  int width;
  int height;

  // circles are stored in orders of magnitude. First one has a radius of 1, then 10, 100, 1000, etc.
  // SDL_Surface *circles;
  // int circle_count;

  int frames;
  int fps;
  unsigned int last_frame;

};

enum font_flags
{
  FONT_LEFT = 0x1,
  FONT_RIGHT = 0x2,
  FONT_CENTER = 0x3,
};

struct renderstate *renderCreate (const char *title, int width, int height);
void renderUniverse (struct renderstate *, struct simulation *,
		     struct universe *);

#endif
