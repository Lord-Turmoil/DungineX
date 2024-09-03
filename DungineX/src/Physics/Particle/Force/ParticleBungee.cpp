#include "DgeX/Physics/Particle/Force/ParticleBungee.h"

#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

void ParticleAnchoredBungee::UpdateForce(Particle* particle, real_t /*delta*/)
{
    DGEX_ASSERT(Anchor);

    Vector3 force = particle->GetPosition() - *Anchor;

    real_t magnitude = force.Magnitude();
    if (magnitude < RestLength)
    {
        return; // if not stretched, no force
    }
    magnitude = (RestLength - magnitude) * SpringConstant;

    force.Normalize();
    force *= magnitude;

    particle->ApplyForce(force);
}

void ParticleBungee::UpdateForce(Particle* particle, real_t /*delta*/)
{
    DGEX_ASSERT(Other);

    Vector3 force = particle->GetPosition() - Other->GetPosition();

    real_t magnitude = force.Magnitude();
    if (magnitude < RestLength)
    {
        return; // if not stretched, no force
    }

    magnitude = (RestLength - magnitude) * SpringConstant;

    force.Normalize();
    force *= magnitude;

    particle->ApplyForce(force);
}

DPHX_END
