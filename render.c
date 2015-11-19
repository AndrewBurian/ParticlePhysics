#include "render.h"

#include <stdio.h>
#include <SDL2/SDL.h>

#include "universe.h"

struct renderstate *renderCreate(const char *title, int width, int height)
{

    struct renderstate *state =
        (struct renderstate *)malloc(sizeof(struct renderstate));

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        return 0;
    }

    if (SDL_CreateWindowAndRenderer
        (width, height, SDL_WINDOW_SHOWN, &state->window,
         &state->renderer) != 0) {
        return 0;
    }

    SDL_SetWindowTitle(state->window, title);

    state->scale = 1.0f;
    state->xPos = 0;
    state->yPos = 0;
    
    return state;
}

void renderUniverse(struct renderstate *state, struct universe *univ)
{

    SDL_Rect rect;
    int i;

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);

    for (i = 0; i < univ->particleCount; i++) {

        if (!univ->particles[i].isActive) {
            continue;
        }

        double x = ((int)univ->particles[i].xPos * state->scale) + state->xPos;
        double y = ((int)univ->particles[i].yPos * state->scale) + state->yPos;

        // rect.x = (int)univ->particles[i].xPos + state->xPos;
        // rect.y = (int)univ->particles[i].yPos + state->yPos;
        rect.x = x;
        rect.y = y;
        rect.w = (int)univ->particles[i].size * state->scale;
        rect.h = (int)univ->particles[i].size * state->scale;

        SDL_RenderFillRect(state->renderer, &rect);

    }

    SDL_RenderPresent(state->renderer);

}
