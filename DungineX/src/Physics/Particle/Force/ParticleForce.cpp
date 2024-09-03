#include "DgeX/Physics/Particle/Force/ParticleForce.h"

#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

void ParticleForce::UpdateForce(Particle* particle, real_t /*delta*/)
{
    if (particle->HasFiniteMass())
    {
        particle->ApplyForce(Force);
    }
}

DPHX_END
