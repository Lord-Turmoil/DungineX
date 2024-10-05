#pragma once

#include "DgeX/Physics/Core/Types/Vector.h"
#include "DgeX/Physics/Particle/Force/ParticleForceGenerator.h"

DPHX_BEGIN

/**
 * @brief To apply gravity force on a particle.
 * @note
 * Although particles can have intrinsic acceleration, using a force generator
 * can be more flexible.
 */
class ParticleGravity : public ParticleForceGenerator
{
public:
    ParticleGravity() = default;

    ParticleGravity(const Vector3& gravity) : Gravity(gravity)
    {
    }

    ~ParticleGravity() override = default;

    void UpdateForce(Particle* particle, real_t delta) override;

public:
    Vector3 Gravity;
};

DPHX_END
