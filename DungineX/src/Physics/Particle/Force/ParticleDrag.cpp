#include "DgeX/Physics/Particle/Force/ParticleDrag.h"

#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

void ParticleDrag::UpdateForce(Particle* particle, real_t /*delta*/)
{
    Vector3 force = particle->GetVelocity();

    // drag = k1 * v + k2 * v^2
    real_t drag = force.Magnitude();
    drag = K1 * drag + K2 * drag * drag;

    // Calculate the final force and apply it
    force.Normalize();
    force *= -drag;

    particle->ApplyForce(force);
}

DPHX_END
