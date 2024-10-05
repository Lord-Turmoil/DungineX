#include "DgeX/Physics/Particle/Force/ParticleFakeSpring.h"
#include "DgeX/Physics/Particle/Particle.h"
#include "DgeX/Utils/Math.h"

DPHX_BEGIN
void ParticleFakeSpring::UpdateForce(Particle* particle, real_t delta)
{
    if (particle->HasInfiniteMass())
    {
        return;
    }

    Vector3 position = particle->GetPosition() - *Anchor;

    // Calculate the constants and check they are in bounds.
    real_t gamma = 0.5f * Math::Sqrt(4 * SpringConstant - Damping * Damping);
    if (gamma == 0.0f)
    {
        return;
    }
    Vector3 c = position * (Damping / ((real_t)2.0 * gamma)) + particle->GetVelocity() * ((real_t)1.0 / gamma);

    // Calculate the target position
    Vector3 target = position * Math::Cos(gamma * delta) + c * Math::Sin(gamma * delta);
    target *= Math::Exp(-0.5f * delta * Damping);

    Vector3 accel =
        (target - position) * ((real_t)1.0 / (delta * delta)) - particle->GetVelocity() * ((real_t)1.0 / delta);

    particle->ApplyForce(accel * particle->GetMass());
}

DPHX_END
