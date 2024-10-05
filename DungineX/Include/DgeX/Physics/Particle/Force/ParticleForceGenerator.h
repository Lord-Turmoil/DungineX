#pragma once

#include "DgeX/Physics/Core/Precision.h"

#include <vector>

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
 * @brief Holds a list of force generators and the particles they apply to.
 */
class ParticleForceRegistry
{
public:
    void Add(Particle* particle, ParticleForceGenerator* forceGenerator);
    void Remove(Particle* particle, ParticleForceGenerator* forceGenerator);
    void Clear();

    void UpdateForce(real_t delta) const;

protected:
    struct ParticleForceRegistration
    {
        Particle* Particle;
        ParticleForceGenerator* ForceGenerator;

        ParticleForceRegistration(DPHX Particle* particle, ParticleForceGenerator* forceGenerator)
            : Particle(particle), ForceGenerator(forceGenerator)
        {
        }

        void UpdateForce(real_t delta) const
        {
            ForceGenerator->UpdateForce(Particle, delta);
        }
    };

    std::vector<ParticleForceRegistration> _registrations;
};

DPHX_END
