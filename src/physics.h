#ifndef PHYSICS_H
#define PHYSICS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#define MAX_PARTICLES 256
#define COLLISION_BEHAVIOR_ELASTIC 1
#define COLLISION_BEHAVIOR_INELASTIC 2

#define PI 3.14159

typedef unsigned int PARTICLE_ID;

class Physics
{
    public:

        struct particle{
            int    isActive;
            int    isStationary;
            double xPos;
            double yPos;
            double xVel;
            double yVel;
            double xForce;
            double yForce;
            double mass;
            double charge;
            double size;
        };

        Physics();

        int         init(size_t maxParticleCount);
        void        resize(size_t maxParticleCount);
        PARTICLE_ID addParticle(double xPos, double yPos, double xVel, double yVel, double mass, double charge, int isStationary);
        void        removeParticle(PARTICLE_ID particle);
        PARTICLE_ID combineParticles(PARTICLE_ID a, PARTICLE_ID b);
        void        bounceParticles(PARTICLE_ID a, PARTICLE_ID b);
        void        applyGravitationForce(PARTICLE_ID a, PARTICLE_ID b);
        void        applyElectromagneticForce(PARTICLE_ID a, PARTICLE_ID b);
        void        tick();
        particle*   getParticle(PARTICLE_ID partID, particle* particle);
        void        setTickTime(double time);
        double      getTickTime();
        size_t      getParticleCount();
        void        applyAll();
        void        collisions(unsigned behavior);
        PARTICLE_ID getParticleAt(double x, double y);

    protected:
        double      findAngle(double xVal, double yVal);
        double      radToDeg(double radians);
        double      degToRad(double degrees);

    private:
        double GRAVITATION = 6.77e-11L;
        double ELECTROSTATIC = 8.987e+9L;
        double DENSITY = 1000;
        double _tickTime = 1;
        size_t  _particleCount;
        int*    _isActive;
        int*    _isStationary;
        double* _xPos;
        double* _yPos;
        double* _xVel;
        double* _yVel;
        double* _xForce;
        double* _yForce;
        double* _mass;
        double* _charge;
        double* _size;
        int     _ready = 0;
};

#endif // PHYSICS_H
