#pragma once

#include "DgeX/Physics/Core/Types/Vector.h"
#include "DgeX/Physics/Particle/Force/ParticleForceGenerator.h"

DPHX_BEGIN

/**
 * @brief
 * Fake spring is to handle hard constraints, it uses Simple Harmonic Motion
 * to calculate the position. Therefore, it has to be connected to a fixed
 * location.
 */
class ParticleFakeSpring : public ParticleForceGenerator
{
public:
    ParticleFakeSpring() : Anchor(nullptr), SpringConstant(0), Damping(0)
    {
    }

    ParticleFakeSpring(Vector3* anchor, real_t springConstant, real_t damping)
        : Anchor(anchor), SpringConstant(springConstant), Damping(damping)
    {
    }

    ~ParticleFakeSpring() override = default;

    void UpdateForce(Particle* particle, real_t delta) override;

public:
    Vector3* Anchor;
    real_t SpringConstant;
    real_t Damping;
};

DPHX_END
