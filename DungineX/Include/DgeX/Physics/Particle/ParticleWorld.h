#pragma once

#include "DgeX/Physics/Particle/ParticleContact.h"
#include "DgeX/Physics/Particle/ParticleForceGenerator.h"

DPHX_BEGIN

class ParticleWorld final
{
public:
    ParticleWorld(int maxContacts, uint32_t iterations = 0);
    ~ParticleWorld();

    /**
     * @brief Step the simulation.
     * @param delta elapsed time since last update
     */
    void Step(real_t delta);

    void AddParticle(Particle* particle)
    {
        _particles.push_back(particle);
    }

    void RemoveParticle(Particle* particle);

    ParticleForceRegistry& GetForceRegistry()
    {
        return _forceRegistry;
    }

    ParticleContactRegistry& GetContactRegistry()
    {
        return _contactRegistry;
    }

private:
    /**
     * @brief Prepare the world for the next frame.
     */
    void _StartFrame();

    /**
     * @brief Integrate to update the position and velocity of particles.
     */
    void _Integrate(real_t delta);

    /**
     * @brief Generate contacts between particles.
     */
    size_t _GenerateContacts();

private:
    std::vector<Particle*> _particles;

    ParticleForceRegistry _forceRegistry;

    ParticleContactRegistry _contactRegistry;
    ParticleContact* _contacts;
    int _maxContacts;

    ParticleContactResolver _resolver;
    uint32_t _iterations;
};

DPHX_END
