#pragma once

#include "DgeX/Physics/Core/Types/Vector.h"
#include "DgeX/Physics/Particle/Force/ParticleForceGenerator.h"

DPHX_BEGIN

/**
 * @brief A spring (spring, bungee) that has one end attached to a fixed point.
 */
class ParticleAnchoredSpring : public ParticleForceGenerator
{
public:
    ParticleAnchoredSpring() : Anchor(nullptr), SpringConstant(0), RestLength(0)
    {
    }

    ParticleAnchoredSpring(Vector3* anchor, real_t springConstant, real_t restLength)
        : Anchor(anchor), SpringConstant(springConstant), RestLength(restLength)
    {
    }

    ~ParticleAnchoredSpring() override = default;

    void UpdateForce(Particle* particle, real_t delta) override;

public:
    /**
     * @note Here we use pointer so that changing the anchor position outside will
     * automatically update all anchors.
     */
    Vector3* Anchor;

    real_t SpringConstant;
    real_t RestLength;
};

/**
 * @brief A spring that connects two particles.
 * @note
 * It is a one-way binding, so... you have to create two force generator
 * to apply the force on both particles.
 */
class ParticleSpring : public ParticleForceGenerator
{
public:
    ParticleSpring() : Other(nullptr), SpringConstant(0), RestLength(0)
    {
    }

    ParticleSpring(Particle* other, real_t springConstant, real_t restLength)
        : Other(other), SpringConstant(springConstant), RestLength(restLength)
    {
    }

    ~ParticleSpring() override = default;

    void UpdateForce(Particle* particle, real_t delta) override;

public:
    Particle* Other;
    real_t SpringConstant;
    real_t RestLength;
};

DPHX_END
