#pragma once

#include "DgeX/Physics/Core/Types/Vector.h"
#include "DgeX/Physics/Particle/Force/ParticleForceGenerator.h"

DPHX_BEGIN

/**
 * @brief To apply constant force on a particle.
 */
class ParticleForce : public ParticleForceGenerator
{
public:
    ParticleForce() = default;

    ParticleForce(const Vector3& force) : Force(force)
    {
    }

    void UpdateForce(Particle* particle, real_t delta) override;

public:
    Vector3 Force;
};

DPHX_END
