#include "render.h"

Render::Render(Physics& physEngine) : _physics(physEngine){}

int Render::init(size_t width, size_t height, char* title){

    SDL_Init(SDL_INIT_VIDEO);

    _window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if(!_window){
        fprintf(stderr, "render - Err: SDL failed to create window: %s\n", SDL_GetError());
        return _renderReady;
	}

	_surface = SDL_GetWindowSurface(_window);
	if(!_surface){
        fprintf(stderr, "render - Err: SDL failed to get surface: %s\n", SDL_GetError());
        return _renderReady;
	}

    _height = height;
    _width = width;
    _renderReady = 1;
    return _renderReady;
}

void Render::cls(){
    if(!_renderReady){
        fprintf(stderr, "render - Err: Render not ready\n");
        return;
    }
    SDL_FillRect(_surface, NULL, 0x000000);
}

void Render::refresh(){
    if(!_renderReady){
        fprintf(stderr, "render - Err: Render not ready\n");
        return;
    }
    SDL_UpdateWindowSurface(_window);
}

void Render::setScalingFactor(double factor){
    _scalingFactor = factor;
}

double Render::getScalingFactor(){
    return _scalingFactor;
}

void Render::all(){
    if(!_renderReady){
        fprintf(stderr, "render - Err: Render not ready\n");
        return;
    }
    size_t count = _physics.getParticleCount();
    PARTICLE_ID i = 0;
    Physics::particle part;

    for(i = 0; i < count; ++i){
        particle(_physics.getParticle(i, &part));
    }
}

void Render::particle(Physics::particle* particle){

    SDL_Rect rect;


    if(!_renderReady){
        fprintf(stderr, "render - Err: Render not ready\n");
        return;
    }
    if(!particle->isActive){
        return;
    }
    // conversion to cartesean coordinates with centered origin
    rect.w = (particle->size * 2) / _scalingFactor;
    if(rect.w < 1){
        rect.w = 1;
    }
    rect.h = rect.w;

    rect.x = (particle->xPos / _scalingFactor) + (0.5 * _width) - (0.5 * rect.w);
    rect.y = (0.5 * _height) - (particle->yPos / _scalingFactor) - (0.5 * rect.h);

    SDL_FillRect(_surface, &rect, 0x00FF00);

}

int Render::getWidth(){
    return _width;
}

int Render::getHeight(){
    return _height;
}

void Render::setXOffset(double offset){
    _xOffset = offset;
}

void Render::setYOffset(double offset){
    _yOffset = offset;
}

double Render::getXOffset(){
    return _xOffset;
}

double Render::getYOffset(){
    return _yOffset;
}
