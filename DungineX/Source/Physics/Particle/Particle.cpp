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
    _acceleration.AddScaledVector(_accumulatedForce, _inverseMass);
    Vector3 acceleration = _acceleration;
    acceleration.AddScaledVector(_accumulatedForce, _inverseMass);

    // Update the velocity based on the new acceleration.
    _velocity.AddScaledVector(acceleration, delta);

    // Simulate simple drag.
    _velocity *= Math::Pow(_damping, delta);

    // Clear the forces.
    ClearAccumulatedForce();
}

void Particle::SetMass(real_t mass)
{
    DGEX_ASSERT(mass > 0);
    SetInverseMass(static_cast<real_t>(1) / mass);
}

real_t Particle::GetMass() const
{
    if (HasInfiniteMass())
    {
        return std::numeric_limits<real_t>::max();
    }
    return static_cast<real_t>(1) / _inverseMass;
}

void Particle::ApplyForce(const Vector3& force)
{
    _accumulatedForce += force;
}

DPHX_END
