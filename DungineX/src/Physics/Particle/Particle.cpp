#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

void Particle::Integrate(real_t delta)
{
    // Particle with infinite mass won't make any movement.
    if (HasInfiniteMass())
    {
        return;
    }

    // First we move the particle.
    _position.AddScaledVector(_velocity, delta);

    // Calculate the changes in acceleration.
    Vector3 acceleration = _acceleration;
    acceleration.AddScaledVector(_accumulatedForce, _inverseMass);

    // Update the velocity based on the new acceleration.
    _velocity.AddScaledVector(acceleration, delta);

    // Simulate simple drag.
    _velocity *= Math::Pow(_damping, delta);

    // Clear the forces.
    ClearAccumulators();
}

void Particle::ApplyForce(const Vector3& force)
{
    _accumulatedForce += force;
}

DPHX_END
