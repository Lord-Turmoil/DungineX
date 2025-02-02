#pragma once

#include "DgeX/Physics/Core/MassWorld.h"

#include "DgeX/Physics/Particle/Contact/ParticleContactGenerator.h"
#include "DgeX/Physics/Particle/Contact/ParticleContactResolver.h"
#include "DgeX/Physics/Particle/Force/ParticleForceGenerator.h"

#include <vector>

DPHX_BEGIN

class Particle;

class ParticleWorld final : public MassWorld
{
public:
    ParticleWorld(uint32_t maxContacts, uint32_t iterations = 0);
    ~ParticleWorld() override;

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

protected:
    /**
     * @brief Step the simulation.
     * @param delta elapsed time since last update
     */
    void Step(real_t delta) override;

private:
    /**
     * @brief Prepare the world for the next frame.
     */
    void _StartFrame() const;

    /**
     * @brief Integrate to update the position and velocity of particles.
     */
    void _Integrate(real_t delta) const;

    /**
     * @brief Generate contacts between particles.
     */
    uint32_t _GenerateContacts() const;

private:
    std::vector<Particle*> _particles;

    ParticleForceRegistry _forceRegistry;

    ParticleContactRegistry _contactRegistry;
    ParticleContact* _contacts;
    uint32_t _maxContacts;

    ParticleContactResolver _resolver;
    uint32_t _iterations;
};

DPHX_END
