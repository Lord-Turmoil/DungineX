#include "DgeX/Physics/Particle/ParticleWorld.h"
#include "DgeX/Physics/Particle/Contact/ParticleContact.h"
#include "DgeX/Physics/Particle/Particle.h"

#include <algorithm>

DPHX_BEGIN

ParticleWorld::ParticleWorld(uint32_t maxContacts, uint32_t iterations)
    : _contacts(new ParticleContact[maxContacts]), _maxContacts(maxContacts), _resolver(iterations),
      _iterations(iterations)
{
}

ParticleWorld::~ParticleWorld()
{
    delete[] _contacts;
}

void ParticleWorld::RemoveParticle(Particle* particle)
{
    auto it = std::find(_particles.begin(), _particles.end(), particle);
    std::swap(*it, _particles.back());
    _particles.pop_back();
}

void ParticleWorld::Step(real_t delta)
{
    _StartFrame();

    _forceRegistry.UpdateForce(delta);

    _Integrate(delta);

    uint32_t count = _GenerateContacts();
    if (count > 0)
    {
        if (_iterations == 0)
        {
            _resolver.SetIterations(count * 2);
        }
        _resolver.Resolve(_contacts, count, delta);
    }
}

void ParticleWorld::_StartFrame() const
{
    for (auto particle : _particles)
    {
        particle->ClearAccumulators();
    }
}

void ParticleWorld::_Integrate(real_t delta) const
{
    for (auto particle : _particles)
    {
        particle->Integrate(delta);
    }
}

uint32_t ParticleWorld::_GenerateContacts() const
{
    return _contactRegistry.AddContact(_contacts, _maxContacts);
}

DPHX_END
