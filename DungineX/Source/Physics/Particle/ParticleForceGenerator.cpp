#include "DgeX/Physics/Particle/ParticleForceGenerator.h"

DPHX_BEGIN

void ParticleGravity::UpdateForce(Particle* particle, real_t delta)
{
    // only apply force to particles with finite mass
    if (particle->HasFiniteMass())
    {
        particle->ApplyForce(_gravity * particle->GetMass());
    }
}

void ParticleDrag::UpdateForce(Particle* particle, real_t delta)
{
    Vector3 force = particle->GetVelocity();

    // drag = k1 * v + k2 * v^2
    real_t drag = force.Magnitude();
    drag = _k1 * drag + _k2 * drag * drag;

    // Calculate the final force and apply it
    force.Normalize();
    force *= -drag;

    particle->ApplyForce(force);
}

void ParticleAnchoredSpring::UpdateForce(Particle* particle, real_t delta)
{
    Vector3 force = particle->GetPosition() - *_anchor;

    real_t magnitude = force.Magnitude();
    magnitude = (_restLength - magnitude) * _springConstant;

    force.Normalize();
    force *= magnitude;

    particle->ApplyForce(force);
}

void ParticleAnchoredBungee::UpdateForce(Particle* particle, real_t delta)
{
    Vector3 force = particle->GetPosition() - *_anchor;

    real_t magnitude = force.Magnitude();
    if (magnitude < _restLength)
    {
        return; // if not stretched, no force
    }

    magnitude = (_restLength - magnitude) * _springConstant;

    force.Normalize();
    force *= magnitude;

    particle->ApplyForce(force);
}

void ParticleFakeSpring::UpdateForce(Particle* particle, real_t delta)
{
    if (particle->HasInfiniteMass())
    {
        return;
    }

    Vector3 position = particle->GetPosition() - *_anchor;

    // Calculate the constants and check they are in bounds.
    real_t gamma = 0.5f * Math::Sqrt(4 * _springConstant - _damping * _damping);
    if (gamma == 0.0f)
    {
        return;
    }
    Vector3 c = position * (_damping / ((real_t)2.0 * gamma)) + particle->GetVelocity() * ((real_t)1.0 / gamma);

    // Calculate the target position
    Vector3 target = position * Math::Cos(gamma * delta) + c * Math::Sin(gamma * delta);
    target *= Math::Exp(-0.5f * delta * _damping);

    Vector3 accel =
        (target - position) * ((real_t)1.0 / (delta * delta)) - particle->GetVelocity() * ((real_t)1.0 / delta);

    particle->ApplyForce(accel * particle->GetMass());
}

void ParticleSpring::UpdateForce(Particle* particle, real_t delta)
{
    Vector3 force = particle->GetPosition() - _other->GetPosition();

    real_t magnitude = force.Magnitude();
    magnitude = (_restLength - magnitude) * _springConstant;

    force.Normalize();
    force *= magnitude;

    particle->ApplyForce(force);
}

void ParticleBungee::UpdateForce(Particle* particle, real_t delta)
{
    Vector3 force = particle->GetPosition() - _other->GetPosition();

    real_t magnitude = force.Magnitude();
    if (magnitude < _restLength)
    {
        return; // if not stretched, no force
    }

    magnitude = (_restLength - magnitude) * _springConstant;

    force.Normalize();
    force *= magnitude;

    particle->ApplyForce(force);
}

void ParticleForceRegistry::Add(Particle* particle, ParticleForceGenerator* forceGenerator)
{
    _registrations.emplace_back(particle, forceGenerator);
}

void ParticleForceRegistry::Remove(Particle* particle, ParticleForceGenerator* forceGenerator)
{
    for (auto it = _registrations.begin(); it != _registrations.end(); ++it)
    {
        if (it->Particle == particle && it->ForceGenerator == forceGenerator)
        {
            _registrations.erase(it);
            break;
        }
    }
}

void ParticleForceRegistry::Clear()
{
    _registrations.clear();
}

void ParticleForceRegistry::UpdateForce(real_t delta) const
{
    for (auto registration : _registrations)
    {
        registration.UpdateForce(delta);
    }
}

DPHX_END
