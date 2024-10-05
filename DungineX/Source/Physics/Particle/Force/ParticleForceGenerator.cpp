#include "DgeX/Physics/Particle/Force/ParticleForceGenerator.h"

DPHX_BEGIN

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
