#pragma once

#include "DgeX/Physics/Core/DWorld.h"
#include "DgeX/Physics/Particle/ParticleContact.h"
#include "DgeX/Physics/Particle/ParticleForceGenerator.h"

DPHX_BEGIN

class ParticleWorld final : public DWorld
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
    void _Step(real_t delta) override;

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
