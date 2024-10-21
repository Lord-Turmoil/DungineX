#pragma once

#include "DgeX/Core/Library/IntrusiveList.h"
#include "DgeX/Physics/Core/Precision.h"

DPHX_BEGIN

class Particle;

class ParticleForceGenerator
{
public:
    virtual ~ParticleForceGenerator() = default;

    /**
     * @brief Update force on a particle.
     * @param particle apply force on this particle
     * @param delta delta time since last update
     */
    virtual void UpdateForce(Particle* particle, real_t delta) = 0;
};

/**
 * @brief Particle force registration entry.
 */
struct ParticleForceRegistration : public IntrusiveListNode
{
    Particle* Particle;
    ParticleForceGenerator* ForceGenerator;

    ParticleForceRegistration() = default;

    ParticleForceRegistration(DPHX Particle* particle, ParticleForceGenerator* forceGenerator)
        : Particle(particle), ForceGenerator(forceGenerator)
    {
    }

    void UpdateForce(real_t delta) const
    {
        ForceGenerator->UpdateForce(Particle, delta);
    }
};

/**
 * @brief Holds a list of force generators and the particles they apply to.
 */
class ParticleForceRegistry
{
public:
    void Add(ParticleForceRegistration* entry);
    void Remove(ParticleForceRegistration* entry);
    void Clear();

    void UpdateForce(real_t delta) const;

protected:
    IntrusiveList<ParticleForceRegistration> _registrations;
};

DPHX_END
