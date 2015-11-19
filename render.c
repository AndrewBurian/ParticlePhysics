#include "render.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "universe.h"

void renderHUD(struct renderstate *render, struct universe *univ);

struct renderstate *renderCreate(const char *title, int width, int height)
{

	struct renderstate *render =
	    (struct renderstate *)malloc(sizeof(struct renderstate));

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		return 0;
	}

	if (TTF_Init() == -1) {
		return 0;
	}

	if (SDL_CreateWindowAndRenderer
	    (width, height, SDL_WINDOW_SHOWN, &render->window,
	     &render->renderer) != 0) {
		return 0;
	}

	SDL_SetWindowTitle(render->window, title);

	render->font_small = TTF_OpenFont("font.ttf", 8);
	render->font_large = TTF_OpenFont("font.ttf", 12);
	if (render->font_small == 0 || render->font_large == 0) {
		return 0;
	}

	render->scale = 1.0f;
	render->xPos = 0;
	render->yPos = 0;
	render->width = width;
	render->height = height;

	return render;
}

void renderUniverse(struct renderstate *render, struct universe *univ)
{
	SDL_Rect rect;
	int i;

	SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, 255);
	SDL_RenderClear(render->renderer);

	SDL_SetRenderDrawColor(render->renderer, 255, 0, 0, 255);

	int centerX = render->width / 2;
	int centerY = render->height / 2;

	for (i = 0; i < univ->particleCount; i++) {

		if (!univ->particles[i].isActive) {
			continue;
		}
		// The position + the screen offset
		double x = (univ->particles[i].xPos + render->xPos);
		double y = (univ->particles[i].yPos + render->yPos);

		// Scale the position
		x *= render->scale;
		y *= render->scale;

		// Move the particles to the middle of the screen.
		x += centerX;
		y += centerY;

		rect.x = x;
		rect.y = y;
		rect.w = (int)(univ->particles[i].size * render->scale);
		rect.h = (int)(univ->particles[i].size * render->scale);

		if (rect.w == 0) {
			rect.w = 1;
		}
		if (rect.h == 0) {
			rect.h = 1;
		}

		SDL_RenderFillRect(render->renderer, &rect);
	}

	// renderHUD(render, univ);

	SDL_RenderPresent(render->renderer);

}

void renderHUD(struct renderstate *render, struct universe *univ)
{
	int scale_size = 50;
	float pixelsPerMeter = 1;

	char text[256];
	// int w, h;
	SDL_Surface *text_surface;
	SDL_Rect text_position;
	SDL_Color text_colour = { 255, 255, 255, 255 };

	SDL_SetRenderDrawColor(render->renderer, 255, 255, 255, 255);

	// Draw the scale in the bottom right corner
	SDL_RenderDrawLine(render->renderer, render->width - 10,
			   render->height - 10, render->width - 10,
			   render->height - 20);
	SDL_RenderDrawLine(render->renderer, render->width - 10,
			   render->height - 15, render->width - 10 - scale_size,
			   render->height - 15);
	SDL_RenderDrawLine(render->renderer, render->width - 10 - scale_size,
			   render->height - 10, render->width - 10 - scale_size,
			   render->height - 20);

	sprintf(text, "%.2fm", render->scale / pixelsPerMeter);
	// TTF_SizeText(render->font_small, text, &w, &h);
	text_surface =
	    TTF_RenderText_Blended(render->font_small, text, text_colour);
	text_position.x =
	    render->width - 10 - (text_surface->w / 2) - (scale_size / 2);
	text_position.y = render->height - 20 - text_surface->h;
	text_position.w = text_surface->w;
	text_position.h = text_surface->h;

	SDL_RenderCopy(render->renderer,
		       SDL_CreateTextureFromSurface(render->renderer,
						    text_surface), 0,
		       &text_position);

	SDL_FreeSurface(text_surface);
}
