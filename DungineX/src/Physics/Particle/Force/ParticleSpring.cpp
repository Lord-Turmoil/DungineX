#include "DgeX/Physics/Particle/Force/ParticleSpring.h"

#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

void ParticleAnchoredSpring::UpdateForce(Particle* particle, real_t /*delta*/)
{
    DGEX_ASSERT(Anchor);

    Vector3 force = particle->GetPosition() - *Anchor;

    real_t magnitude = force.Magnitude();
    magnitude = (RestLength - magnitude) * SpringConstant;

    force.Normalize();
    force *= magnitude;

    particle->ApplyForce(force);
}

void ParticleSpring::UpdateForce(Particle* particle, real_t /*delta*/)
{
    DGEX_ASSERT(Other, "Other is nullptr");

    Vector3 force = particle->GetPosition() - Other->GetPosition();

    real_t magnitude = force.Magnitude();
    magnitude = (RestLength - magnitude) * SpringConstant;

    force.Normalize();
    force *= magnitude;

    particle->ApplyForce(force);
}

DPHX_END
