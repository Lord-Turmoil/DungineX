#pragma once

#include "DgeX/Physics/Particle/Force/ParticleSpring.h"

DPHX_BEGIN

/**
 * @brief A bungee can be a special spring that doesn't expand.
 */
class ParticleAnchoredBungee : public ParticleAnchoredSpring
{
public:
    ParticleAnchoredBungee() = default;

    ParticleAnchoredBungee(Vector3* anchor, real_t springConstant, real_t restLength)
        : ParticleAnchoredSpring(anchor, springConstant, restLength)
    {
    }

    void UpdateForce(Particle* particle, real_t delta) override;
};

/**
 * @brief Well, a bungee is a spring that doesn't expand.
 */
class ParticleBungee : public ParticleSpring
{
public:
    ParticleBungee() = default;

    ParticleBungee(Particle* other, real_t springConstant, real_t restLength)
        : ParticleSpring(other, springConstant, restLength)
    {
    }

    void UpdateForce(Particle* particle, real_t delta) override;
};

DPHX_END
