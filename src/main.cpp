#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "physics.h"
#include "render.h"

void runEngine();
void runRender();
void addParticle(int x, int y);
void removeParticle(int x, int y);
void printParticle(int x, int y);

Physics physicsEngine;
Render renderEngine(physicsEngine);

int main()
{
    int running = 1;
    int pause = 0;

    SDL_Event e;

    renderEngine.init(1080, 720, "Particles!");
    renderEngine.cls();
    renderEngine.refresh();

    physicsEngine.init(100);

    while (running){
		//check if window is closed
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				running = 0;
			}
			else if (e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){

                case SDLK_ESCAPE:
                    running = 0;
                    break;

                case SDLK_COMMA:
                    physicsEngine.setTickTime(physicsEngine.getTickTime() / 2);
                    printf("Tick time set to %f s/tick\n", physicsEngine.getTickTime());
                    fflush(stdout);
                    break;

                case SDLK_PERIOD:
                    physicsEngine.setTickTime(physicsEngine.getTickTime() * 2);
                    printf("Tick time set to %f s/tick\n", physicsEngine.getTickTime());
                    fflush(stdout);
                    break;

                case SDLK_EQUALS:
                    renderEngine.setScalingFactor(renderEngine.getScalingFactor() / 2);
                    printf("Scale set to %f m/px\n", renderEngine.getScalingFactor());
                    break;

                case SDLK_MINUS:
                    renderEngine.setScalingFactor(renderEngine.getScalingFactor() * 2);
                    printf("Scale set to %f m/px\n", renderEngine.getScalingFactor());
                    break;

                case SDLK_LEFT:
                    renderEngine.setXOffset(renderEngine.getXOffset() - (50 * renderEngine.getScalingFactor()));
                    break;

                case SDLK_RIGHT:
                    renderEngine.setXOffset(renderEngine.getXOffset() + (50 * renderEngine.getScalingFactor()));
                    break;

                case SDLK_UP:
                    renderEngine.setYOffset(renderEngine.getYOffset() + (50 * renderEngine.getScalingFactor()));
                    break;

                case SDLK_DOWN:
                    renderEngine.setYOffset(renderEngine.getYOffset() - (50 * renderEngine.getScalingFactor()));
                    break;

                case SDLK_p:
                    pause = (pause ? 0 : 1);
                    printf("%s\n", (pause ? "Paused" : "Resumed"));
                    break;
                }
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN){

                switch(e.button.button){

                case SDL_BUTTON_LEFT:
                    addParticle(e.button.x, e.button.y);
                    break;

                case SDL_BUTTON_RIGHT:
                    removeParticle(e.button.x, e.button.y);
                    break;

                case SDL_BUTTON_MIDDLE:
                    printParticle(e.button.x, e.button.y);
                    break;
                }
			}
		}
		if(!pause){
            runEngine();
        }
        runRender();
    }

    return 0;
}

void runEngine(){

    physicsEngine.applyAll();
    physicsEngine.tick();
    physicsEngine.collisions(COLLISION_BEHAVIOR_INELASTIC);

}

void runRender(){

    renderEngine.cls();
    renderEngine.all();
    renderEngine.refresh();

}

void addParticle(int x, int y){

    double particleX = (x - (0.5 * renderEngine.getWidth())) * renderEngine.getScalingFactor();
    double particleY = ((0.5 * renderEngine.getHeight()) - y) * renderEngine.getScalingFactor();

    printf("Creating particle at (%d, %d)\n", (int)particleX, (int)particleY);

    physicsEngine.addParticle(particleX, particleY, 0, 0, 50, 0.0000001 * ((rand() % 3) - 1), 0);
    physicsEngine.collisions(COLLISION_BEHAVIOR_INELASTIC);
}

void removeParticle(int x, int y){

    double particleX = (x - (0.5 * renderEngine.getWidth())) * renderEngine.getScalingFactor();
    double particleY = ((0.5 * renderEngine.getHeight()) - y) * renderEngine.getScalingFactor();

    printf("Removing particle at (%d, %d)\n", (int)particleX, (int)particleY);
    physicsEngine.removeParticle(physicsEngine.getParticleAt(particleX, particleY));
}

void printParticle(int x, int y){

    Physics::particle particle = {0};

    double particleX = (x - (0.5 * renderEngine.getWidth())) * renderEngine.getScalingFactor();
    double particleY = ((0.5 * renderEngine.getHeight()) - y) * renderEngine.getScalingFactor();

    physicsEngine.getParticle(physicsEngine.getParticleAt(particleX, particleY), &particle);

    if(!particle.isActive){
        printf("Not a particle\n");
        return;
    }

    printf("Particle Info:\n");
    printf("   Mass  (Kg): %e\n", particle.mass);
    printf(" Charge  (C):  %e\n", particle.charge);
    printf("Position (m): x:%e\n", particle.xPos);
    printf("              y:%e\n", particle.yPos);
    printf("Velocity(m/s): x:%e\n", particle.xVel);
    printf("               y:%e\n", particle.yVel);
    fflush(stdout);

}

