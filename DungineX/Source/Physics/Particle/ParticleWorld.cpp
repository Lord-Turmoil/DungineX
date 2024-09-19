#include "DgeX/Physics/Particle/ParticleWorld.h"

DPHX_BEGIN

ParticleWorld::ParticleWorld(int maxContacts, uint32_t iterations)
    : _contacts(new ParticleContact[maxContacts]), _maxContacts(maxContacts), _resolver(iterations),
      _iterations(iterations)
{
}

ParticleWorld::~ParticleWorld()
{
    delete[] _contacts;
}

void ParticleWorld::Step(real_t delta)
{
    _StartFrame();

    _forceRegistry.UpdateForce(delta);

    _Integrate(delta);

    size_t count = _GenerateContacts();
    if (count > 0)
    {
        if (_iterations == 0)
        {
            _resolver.SetIterations(static_cast<uint32_t>(count * 2));
        }
        _resolver.Resolve(_contacts, count, delta);
    }
}

void ParticleWorld::RemoveParticle(Particle* particle)
{
    auto it = std::find(_particles.begin(), _particles.end(), particle);
    std::swap(*it, _particles.back());
    _particles.pop_back();
}

void ParticleWorld::_StartFrame()
{
    for (auto particle : _particles)
    {
        particle->ClearAccumulators();
    }
}

void ParticleWorld::_Integrate(real_t delta)
{
    for (auto particle : _particles)
    {
        particle->Integrate(delta);
    }
}

size_t ParticleWorld::_GenerateContacts()
{
    return _contactRegistry.AddContact(_contacts, _maxContacts);
}

DPHX_END
