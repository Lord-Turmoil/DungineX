#pragma once

#include "DgeX/Physics/Particle/Force/ParticleSpring.h"

DPHX_BEGIN

/**
 * @brief
 * In particle world, the spring will stretch and compress forever as we don't
 * have friction. In this case, we provide spring with absorber to add damping
 * to this process so that the spring will stop moving after a while.
 */
class ParticleAbsorber : public ParticleSpring
{
public:
    ParticleAbsorber() : Damping(0)
    {
    }

    ParticleAbsorber(Particle* other, real_t springConstant, real_t restLength, real_t damping)
        : ParticleSpring(other, springConstant, restLength), Damping(damping)
    {
    }

    void UpdateForce(Particle* particle, real_t delta) override;

public:
    real_t Damping;
};

DPHX_END
