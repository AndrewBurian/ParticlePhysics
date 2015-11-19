#include "render.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "universe.h"

void renderHUD(struct renderstate *render, struct simulation *sim,
	       struct universe *univ);
void renderHotParticle(struct renderstate *render, struct simulation *sim,
		       struct universe *univ);
void renderText(struct renderstate *render, TTF_Font * font, const char *text,
		int x, int y, int flags, SDL_Color colour);

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

	// TODO(jordan): Generate different sized circles to draw the particles as.
	// circle_count = 5;

	return render;
}

void renderUniverse(struct renderstate *render, struct simulation *sim,
		    struct universe *univ)
{
	SDL_Rect rect;
	int i;

	SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, 255);
	SDL_RenderClear(render->renderer);

	SDL_SetRenderDrawColor(render->renderer, 255, 0, 0, 255);

	for (i = 0; i < univ->particleCount; i++) {

		if (!univ->particles[i].isActive) {
			continue;
		}
		double x =
		    ((univ->particles[i].xPos + render->xPos) * render->scale) +
		    (render->width / 2);
		double y =
		    ((univ->particles[i].yPos + render->yPos) * render->scale) +
		    (render->height / 2);

		rect.w =
		    (int)(univ->particles[i].size * 2 * render->scale *
			  univ->scale);
		rect.h =
		    (int)(univ->particles[i].size * 2 * render->scale *
			  univ->scale);
		rect.x = x - (rect.w / 2);
		rect.y = y - (rect.h / 2);

		if (rect.w == 0) {
			rect.w = 1;
		}
		if (rect.h == 0) {
			rect.h = 1;
		}

		SDL_RenderFillRect(render->renderer, &rect);
	}

	renderHotParticle(render, sim, univ);
	renderHUD(render, sim, univ);

	SDL_RenderPresent(render->renderer);

}

void renderHUD(struct renderstate *render, struct simulation *sim,
	       struct universe *univ)
{

	float pixelsPerMeter = univ->scale * render->scale;
	int scale_size = 100;
	char text[256];
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

	sprintf(text, "%.2fm", (1 / pixelsPerMeter) * scale_size);

	renderText(render, render->font_small, text,
		   render->width - 10 - (scale_size / 2), render->height - 25,
		   FONT_CENTER, text_colour);

	// Draw the pause symbol
	if (sim->paused) {
		SDL_Rect rect;

		SDL_SetRenderDrawColor(render->renderer, 255, 0, 0, 255);

		rect.x = 10;
		rect.y = 10;
		rect.w = 6;
		rect.h = 16;
		SDL_RenderFillRect(render->renderer, &rect);

		rect.x = 20;
		SDL_RenderFillRect(render->renderer, &rect);

	}
}

void renderHotParticle(struct renderstate *render, struct simulation *sim,
		       struct universe *univ)
{
	struct particle p;
	SDL_Color text_selected = { 255, 255, 0, 255 };
	SDL_Color text_unselected = { 255, 255, 255, 255 };
	char text[256];

	if (sim->hotParticle == -1) {
		return;
	}

	p = univ->particles[sim->hotParticle];

	double x =
	    ((p.xPos + render->xPos) * render->scale) + (render->width / 2);
	double y =
	    ((p.yPos + render->yPos) * render->scale) + (render->height / 2);

	double size = p.size * 2 * render->scale * univ->scale;

	// Draw X on the hot particle
	SDL_SetRenderDrawColor(render->renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(render->renderer, x - (size / 4), y - (size / 4),
			   x + (size / 4), y + (size / 4));
	SDL_RenderDrawLine(render->renderer, x + (size / 4), y - (size / 4),
			   x - (size / 4), y + (size / 4));

	// Draw velocity
	double distance = sqrt(pow(p.xVel, 2) + pow(p.yVel, 2));
	double scale = 100 * render->scale * univ->scale;
	SDL_SetRenderDrawColor(render->renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(render->renderer, x, y, x + (p.xVel * scale),
			   y + (p.yVel * scale));
	sprintf(text, "%f m/s", distance);
	renderText(render, render->font_small, text, x + (p.xVel * scale),
		   y + (p.yVel * scale), FONT_LEFT, text_unselected);

	// Draw the options and current one.
	renderText(render, render->font_large, "Velocity", x,
		   y + (size / 2) + 10, FONT_LEFT,
		   sim->hotParticleState ==
		   HOTPARTICLE_VELOCITY ? text_selected : text_unselected);
	sprintf(text, "Mass: %.2fkg", p.mass);
	renderText(render, render->font_large, text, x, y + (size / 2) + 25,
		   FONT_LEFT,
		   sim->hotParticleState ==
		   HOTPARTICLE_MASS ? text_selected : text_unselected);
	sprintf(text, "Charge: %.2fC", p.charge);
	renderText(render, render->font_large, text, x, y + (size / 2) + 40,
		   FONT_LEFT,
		   sim->hotParticleState ==
		   HOTPARTICLE_CHARGE ? text_selected : text_unselected);

}

void renderText(struct renderstate *render, TTF_Font * font, const char *text,
		int x, int y, int flags, SDL_Color colour)
{
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect text_position;

	text_surface = TTF_RenderText_Blended(font, text, colour);
	text_position.x = x;
	text_position.y = y;
	text_position.w = text_surface->w;
	text_position.h = text_surface->h;

	if (flags == FONT_CENTER) {
		text_position.x -= (text_surface->w / 2);
		text_position.y -= (text_surface->h / 2);
	} else if (flags == FONT_RIGHT) {
		text_position.x -= text_surface->w;
	}

	text_texture = SDL_CreateTextureFromSurface(render->renderer,
						    text_surface);
	SDL_RenderCopy(render->renderer, text_texture, 0, &text_position);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
}
