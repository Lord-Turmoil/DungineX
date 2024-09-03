#include "DgeX/Physics/Particle/Force/ParticleAbsorber.h"

#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

void ParticleAbsorber::UpdateForce(Particle* particle, real_t /*delta*/)
{
    Vector3 force = particle->GetPosition() - Other->GetPosition();

    real_t springForce = force.Magnitude();
    springForce = (RestLength - springForce) * SpringConstant;

    force.Normalize();
    real_t dampingForce = (particle->GetVelocity() - Other->GetVelocity()) * force * Damping;

    force *= (springForce - dampingForce);

    particle->ApplyForce(force);
}

DPHX_END
