#include "DgeX/Physics/Particle/Force/ParticleForceGenerator.h"

DPHX_BEGIN

void ParticleForceRegistry::Add(ParticleForceRegistration* entry)
{
    _registrations.PushBack(entry);
}

void ParticleForceRegistry::Remove(ParticleForceRegistration* entry)
{
    _registrations.Erase(entry);
}

void ParticleForceRegistry::Clear()
{
    _registrations.Clear();
}

void ParticleForceRegistry::UpdateForce(real_t delta) const
{
    for (const auto& registration : _registrations)
    {
        registration.UpdateForce(delta);
    }
}

DPHX_END
