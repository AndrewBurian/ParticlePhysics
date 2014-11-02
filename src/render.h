#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include "physics.h"

class Render
{
    public:
        Render(Physics& physEngine);
        int     init(size_t width, size_t height, char* title);
        void    setScalingFactor(double factor);
        double  getScalingFactor();
        void    all();
        void    particle(Physics::particle* particle);
        void    cls();
        void    refresh();
        int     getHeight();
        int     getWidth();
        double  getXOffset();
        void    setXOffset(double offset);
        double  getYOffset();
        void    setYOffset(double offset);

    protected:
    private:
        Physics &_physics;
        SDL_Window* _window;
        SDL_Surface* _surface;
        double _scalingFactor = 1;
        int _renderReady = 0;
        int _height = 0;
        int _width = 0;
        double _xOffset = 0;
        double _yOffset = 0;
        double _colorThreshold = 0.0000001L;
};

#endif // RENDER_H
