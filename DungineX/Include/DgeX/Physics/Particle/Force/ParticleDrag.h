#pragma once

#include "DgeX/Physics/Particle/Force/ParticleForceGenerator.h"

DPHX_BEGIN

/**
 * @brief To apply drag force on a particle.
 * @note
 * Although particles can have intrinsic drag (damping), using a force generator
 * can a fine-grained control.
 */
class ParticleDrag : public ParticleForceGenerator
{
public:
    ParticleDrag() : K1(0), K2(0)
    {
    }

    ParticleDrag(real_t k1, real_t k2) : K1(k1), K2(k2)
    {
    }

    ~ParticleDrag() override = default;

    void UpdateForce(Particle* particle, real_t delta) override;

public:
    real_t K1;
    real_t K2;
};

DPHX_END
