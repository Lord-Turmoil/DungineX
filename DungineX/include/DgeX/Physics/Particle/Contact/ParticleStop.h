#pragma once

#include "DgeX/Physics/Particle/Contact/ParticleLink.h"

DPHX_BEGIN

/**
 * @brief Half rod restricts the minimum distance between two particles.
 */
class ParticleAnchoredStop : public ParticleAnchoredLink
{
public:
    ParticleAnchoredStop() : MinLength(0), Restitution(0)
    {
    }

    ParticleAnchoredStop(Particle* particle, Vector3* anchor, real_t minLength, real_t restitution)
        : ParticleAnchoredLink(particle, anchor), MinLength(minLength), Restitution(restitution)
    {
    }

    ~ParticleAnchoredStop() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

public:
    real_t MinLength;
    real_t Restitution;
};

/**
 * @brief Restricts the minimum distance between two particles.
 */
class ParticleStop : public ParticleLink
{
public:
    ParticleStop() : MinLength(0), Restitution(0)
    {
    }

    ParticleStop(Particle* first, Particle* second, real_t minLength, real_t restitution)
        : ParticleLink(first, second), MinLength(minLength), Restitution(restitution)
    {
    }

    ~ParticleStop() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

public:
    real_t MinLength;
    real_t Restitution;
};

DPHX_END
