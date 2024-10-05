#include "DgeX/Physics/Particle/Force/ParticleGravity.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

void ParticleGravity::UpdateForce(Particle* particle, real_t delta)
{
    // only apply force to particles with finite mass
    if (particle->HasFiniteMass())
    {
        particle->ApplyForce(Gravity * particle->GetMass());
    }
}

DPHX_END
