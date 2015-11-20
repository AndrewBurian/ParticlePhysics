#include "physics.h"
#include "universe.h"
#include <math.h>

void applyGravityPart (struct particle *, struct particle *);
void applyElectricPart (struct particle *, struct particle *);
int collideParticles (struct particle *, struct particle *);

void
physicsApply (struct universe *univ)
{
  int i;

  for (i = 0; i < univ->fidelity; i++)
    {
      applyGravity (univ);
      applyElectric (univ);
      applyMovement (univ);
      applyCollision (univ);
    }
}

void
applyGravity (struct universe *univ)
{
  int i, j = 0;

  for (i = 0; i < univ->nextParticle; i++)
    {
      if (!univ->particles[i].isActive)
	{
	  continue;
	}

      for (j = i + 1; j < univ->nextParticle; j++)
	{
	  if (!univ->particles[j].isActive)
	    {
	      continue;
	    }

	  applyGravityPart (&univ->particles[i], &univ->particles[j]);

	}

    }
}

void
applyElectric (struct universe *univ)
{

  int i, j = 0;

  for (i = 0; i < univ->nextParticle; i++)
    {
      if (!univ->particles[i].isActive)
	{
	  continue;
	}

      for (j = i + 1; j < univ->nextParticle; j++)
	{
	  if (!univ->particles[j].isActive)
	    {
	      continue;
	    }

	  applyElectricPart (&univ->particles[i], &univ->particles[j]);

	}

    }
}

void
applyGravityPart (struct particle *a, struct particle *b)
{

  double xDist = a->xPos - b->xPos;
  double yDist = a->yPos - b->yPos;
  double distance = sqrt (pow (xDist, 2) + pow (yDist, 2));

  double totalForce =
    GRAVITIATION * ((a->mass * b->mass) / pow (distance, 2));

  a->xForce -= totalForce * xDist / distance;
  a->yForce -= totalForce * yDist / distance;

  b->xForce += totalForce * xDist / distance;
  b->yForce += totalForce * yDist / distance;

}

void
applyElectricPart (struct particle *a, struct particle *b)
{

  double xDist = a->xPos - b->xPos;
  double yDist = a->yPos - b->yPos;
  double distance = sqrt (pow (xDist, 2) + pow (yDist, 2));

  double totalForce =
    ELECTROSTATIC * ((a->charge * b->charge) / pow (distance, 2));

  a->xForce -= totalForce * xDist / distance;
  a->yForce -= totalForce * yDist / distance;

  b->xForce += totalForce * xDist / distance;
  b->yForce += totalForce * yDist / distance;

}

void
applyMovement (struct universe *univ)
{

  int i = 0;
  struct particle *p;

  double xAccel, yAccel;
  double tickSpeed = univ->speed / univ->fidelity;

  for (i = 0; i < univ->nextParticle; ++i)
    {

      p = &univ->particles[i];

      if (!p->isActive || p->isStationary)
	{
	  continue;
	}
      // F = ma
      // a = F / m
      xAccel = (p->xForce / p->mass);
      yAccel = (p->yForce / p->mass);

      // update position
      // x = v t + 0.5 a t^2
      p->xPos +=
	(p->xVel * tickSpeed) + (0.5 * xAccel * (pow (tickSpeed, 2)));
      p->yPos +=
	(p->yVel * tickSpeed) + (0.5 * yAccel * (pow (tickSpeed, 2)));

      // update velocities
      p->xVel += xAccel * tickSpeed;
      p->yVel += yAccel * tickSpeed;

      p->xForce = 0;
      p->yForce = 0;
    }
}

void
applyCollision (struct universe *univ)
{

  int i, j = 0;

  for (i = 0; i < univ->nextParticle; i++)
    {
      if (!univ->particles[i].isActive)
	{
	  continue;
	}

      for (j = i + 1; j < univ->nextParticle; j++)
	{
	  if (!univ->particles[j].isActive)
	    {
	      continue;
	    }

	  if (collideParticles (&univ->particles[i], &univ->particles[j]))
	    {
	      deleteParticle (univ, &univ->particles[j]);
	    }

	}

    }
}

int
collideParticles (struct particle *a, struct particle *b)
{
  double distance =
    sqrt (pow (a->xPos - b->xPos, 2) + pow (a->yPos - b->yPos, 2));

  // no collision check
  if (distance > a->size + b->size)
    {
      return 0;
    }
  // combine into particle a

  // add charge
  a->charge += b->charge;

  // mass-weighted new position
  a->xPos = ((a->xPos * a->mass) + (b->xPos * b->mass)) / (a->mass + b->mass);
  a->yPos = ((a->yPos * a->mass) + (b->yPos * b->mass)) / (a->mass + b->mass);

  // conservation of momentum
  a->xVel = ((a->xVel * a->mass) + (b->xVel * b->mass)) / (a->mass + b->mass);
  a->yVel = ((a->yVel * a->mass) + (b->yVel * b->mass)) / (a->mass + b->mass);

  // consertation of mass
  a->mass += b->mass;

  // stationary particles remain so
  a->isStationary += b->isStationary;

  // size is a function of density and mass
  setParticleSize (a);

  return 1;
}

void
setParticleSize (struct particle *p)
{
  p->size = sqrt (p->mass / DENSITY / M_PI);
}
