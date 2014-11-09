#include "physics.h"

Physics::Physics(){

}

int Physics::init(size_t maxParticleCount){

    // If the max count is < 0, use MAX_PARTICLES
    if(maxParticleCount <= 0){
        maxParticleCount = MAX_PARTICLES;
    }
    if(maxParticleCount > MAX_PARTICLES){
        maxParticleCount = MAX_PARTICLES;
    }

    // if physics has already been started
    if(_ready){
        // see if this is an attempted resize
        if(maxParticleCount != _particleCount){
            resize(maxParticleCount);
        }

        return _ready;
    }

    _particleCount = maxParticleCount;

    _isActive = (int*)malloc(sizeof(int) * _particleCount);
    bzero(_isActive, sizeof(int) * _particleCount);

    _isStationary = (int*)malloc(sizeof(int) * _particleCount);
    bzero(_isStationary, sizeof(int) * _particleCount);

    _xPos = (double*)malloc(sizeof(double) * _particleCount);
    bzero(_xPos, sizeof(double) * _particleCount);

    _yPos = (double*)malloc(sizeof(double) * _particleCount);
    bzero(_yPos, sizeof(double) * _particleCount);

    _xVel = (double*)malloc(sizeof(double) * _particleCount);
    bzero(_xVel, sizeof(double) * _particleCount);

    _yVel = (double*)malloc(sizeof(double) * _particleCount);
    bzero(_yVel, sizeof(double) * _particleCount);

    _mass = (double*)malloc(sizeof(double) * _particleCount);
    bzero(_mass, sizeof(double) * _particleCount);

    _charge = (double*)malloc(sizeof(double) * _particleCount);
    bzero(_charge, sizeof(double) * _particleCount);

    _xForce = (double*)malloc(sizeof(double) * _particleCount);

    _yForce = (double*)malloc(sizeof(double) * _particleCount);

    _size = (double*)malloc(sizeof(double) * _particleCount);

    _ready = 1;

    return _ready;
}

void Physics::resize(size_t maxParticleCount){
    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return;
    }

    //TODO: realloc physics space
}

PARTICLE_ID Physics::addParticle(double xPos, double yPos, double xVel, double yVel, double mass, double charge, int isStationary){
    PARTICLE_ID i = 0;

    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return -1;
    }

    // find an available particle
    for(i = 0; i < _particleCount; ++i){
        if(!_isActive[i]){
            break;
        }
    }
    // no space found
    if(i == _particleCount){
        fprintf(stderr, "physics: Err - paticals full\n");
        return -1;
    }

    _isActive[i] = 1;

    _xPos[i] = xPos;
    _yPos[i] = yPos;
    _xVel[i] = xVel;
    _yVel[i] = yVel;
    _xForce[i] = 0;
    _yForce[i] = 0;
    _mass[i] = mass;
    _charge[i] = charge;
    _isStationary[i] = isStationary;
    _size[i] = sqrt((_mass[i] / DENSITY)/ PI);

    return i;
}

void Physics::removeParticle(PARTICLE_ID particle){
    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return;
    }

    if(particle < 0 || particle >= _particleCount){
        return;
    }
    _isActive[particle] = 0;
    return;
}

PARTICLE_ID Physics::combineParticles(PARTICLE_ID a, PARTICLE_ID b){

    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return -1;
    }
    if(!_isActive[a] || !_isActive[b]){
        fprintf(stderr, "physics: Err - combining inactive particles\n");
        return -1;
    }

    _charge[a] += _charge[b];
    _xPos[a] = ((_xPos[a] * _mass[a]) + (_xPos[b] * _mass[b])) / (_mass[a] + _mass[b]);
    _yPos[a] = ((_yPos[a] * _mass[a]) + (_yPos[b] * _mass[b])) / (_mass[a] + _mass[b]);
    _xVel[a] = ((_xVel[a] * _mass[a]) + (_xVel[b] * _mass[b])) / (_mass[a] + _mass[b]);
    _yVel[a] = ((_yVel[a] * _mass[a]) + (_yVel[b] * _mass[b])) / (_mass[a] + _mass[b]);
    _mass[a] += _mass[b];
    _size[a] = sqrt((_mass[a] / DENSITY)/ PI);
    _isStationary[a] += _isStationary[b];
    removeParticle(b);
    return a;
}

void Physics::applyGravitationForce(PARTICLE_ID a, PARTICLE_ID b){

    double xDist = _xPos[a] - _xPos[b];
    double yDist = _yPos[a] - _yPos[b];
    double distance = 0;

    double totalForce = 0;

    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return;
    }
    if(!_isActive[a] || !_isActive[b]){
        fprintf(stderr, "physics: Err - applying gravitation between inactive particles\n");
        return;
    }

    distance = sqrt((xDist * xDist) + (yDist * yDist));

    totalForce = GRAVITATION * ((_mass[a] * _mass[b]) / (distance * distance));

    _xForce[a] -= totalForce * xDist / distance;
    _yForce[a] -= totalForce * yDist / distance;

    _xForce[b] += totalForce * xDist / distance;
    _yForce[b] += totalForce * yDist / distance;
}

void Physics::applyElectromagneticForce(PARTICLE_ID a, PARTICLE_ID b){

    double xDist = _xPos[a] - _xPos[b];
    double yDist = _yPos[a] - _yPos[b];
    double distance = 0;

    double totalForce = 0;

    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return;
    }
    if(!_isActive[a] || !_isActive[b]){
        fprintf(stderr, "physics: Err - applying gravitation between inactive particles\n");
        return;
    }

    distance = sqrt((xDist * xDist) + (yDist * yDist));

    totalForce = ELECTROSTATIC * ((_charge[a] * _charge[b]) / (distance * distance));

    _xForce[a] += totalForce * xDist / distance;
    _yForce[a] += totalForce * yDist / distance;

    _xForce[b] -= totalForce * xDist / distance;
    _yForce[b] -= totalForce * yDist / distance;
}

void Physics::tick(){
    // x = v_0 t + 1/2at^2
    PARTICLE_ID i = 0;

    double xAccel = 0;
    double yAccel = 0;

    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return;
    }

    for(i = 0; i < _particleCount; ++i){
        if(!_isActive[i] || _isStationary[i]){
            continue;
        }

        xAccel = (_xForce[i] / _mass[i]);
        yAccel = (_yForce[i] / _mass[i]);

        //update position
        _xPos[i] += (_xVel[i] * _tickTime) + ( 0.5 * xAccel * (_tickTime * _tickTime));
        _yPos[i] += (_yVel[i] * _tickTime) + ( 0.5 * yAccel * (_tickTime * _tickTime));

        // update velocities
        _xVel[i] += xAccel * _tickTime;
        _yVel[i] += yAccel * _tickTime;

        _xForce[i] = 0;
        _yForce[i] = 0;

    }
}

Physics::particle* Physics::getParticle(PARTICLE_ID partID, particle* particle){

    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return 0;
    }
    if(partID < 0 || partID >= _particleCount){
        return 0;
    }

    particle->charge = _charge[partID];
    particle->isActive = _isActive[partID];
    particle->isStationary = _isStationary[partID];
    particle->mass = _mass[partID];
    particle->xForce = _xForce[partID];
    particle->yForce = _yForce[partID];
    particle->xPos = _xPos[partID];
    particle->yPos = _yPos[partID];
    particle->xVel = _xVel[partID];
    particle->yVel = _yVel[partID];
    particle->size = _size[partID];

    return particle;
}

void Physics::setTickTime(double time){
    _tickTime = time;
}

double Physics::getTickTime(){
    return _tickTime;
}

size_t Physics::getParticleCount(){
    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return 0;
    }
    return _particleCount;
}

void Physics::applyAll(){
    PARTICLE_ID i, j;

    if(!_ready){
        fprintf(stderr, "physics: Err - not initialized\n");
        return;
    }

    for(i = 0; i < _particleCount; ++i){
        for(j = i + 1; j < _particleCount; ++j){
            if(_isActive[i] && _isActive[j]){
                applyGravitationForce(i, j);
                applyElectromagneticForce(i, j);
            }
        }
    }
}

void Physics::collisions(unsigned behavior){

    PARTICLE_ID i, j;

    double dist;

    if(behavior != COLLISION_BEHAVIOR_ELASTIC && behavior != COLLISION_BEHAVIOR_INELASTIC){
        fprintf(stderr, "physics: Err - Invalid behavior\n");
        return;
    }

    if(behavior == COLLISION_BEHAVIOR_ELASTIC){
        fprintf(stderr, "physics: Err - Elastic not yet implemented\n");
        // TODO elastic
        return;
    }

    for(i = 0; i < _particleCount; ++i){
        for(j = i + 1; j < _particleCount; ++j){
            if(_isActive[i] && _isActive[j]){

                dist = (_xPos[i] - _xPos[j]) * (_xPos[i] - _xPos[j]); // a^2
                dist += (_yPos[i] - _yPos[j]) * (_yPos[i] - _yPos[j]); // *b^2
                dist = sqrt(dist); // = c^2
                if(dist < (_size[i] + _size[j])){
                    combineParticles(i, j);
                }

            }
        }
    }

}

PARTICLE_ID Physics::getParticleAt(double x, double y){
    PARTICLE_ID i = 0;

    for(i = 0; i < _particleCount; ++i){
        if(_isActive[i]){
            if(x < _xPos[i] + _size[i] && x > _xPos[i] - _size[i] && y < _yPos[i] + _size[i] && y > _yPos[i] - _size[i]){
                return i;
            }
        }
    }

    return -1;
}
