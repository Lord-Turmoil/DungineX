#pragma once

#include "DgeX/Physics/Particle/Contact/ParticleLink.h"

DPHX_BEGIN

/**
 * @brief Restricts the maximum distance between two particles.
 */
class ParticleAnchoredCable : public ParticleAnchoredLink
{
public:
    ParticleAnchoredCable() : MaxLength(0), Restitution(0)
    {
    }

    ParticleAnchoredCable(Particle* particle, Vector3* anchor, real_t maxLength, real_t restitution)
        : ParticleAnchoredLink(particle, anchor), MaxLength(maxLength), Restitution(restitution)
    {
    }

    ~ParticleAnchoredCable() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

public:
    real_t MaxLength;
    real_t Restitution;
};

/**
 * @brief Restricts the maximum distance between two particles.
 */
class ParticleCable : public ParticleLink
{
public:
    ParticleCable() : MaxLength(0), Restitution(0)
    {
    }

    ParticleCable(Particle* first, Particle* second, real_t maxLength, real_t restitution)
        : ParticleLink(first, second), MaxLength(maxLength), Restitution(restitution)
    {
    }

    ~ParticleCable() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

public:
    real_t MaxLength;
    real_t Restitution;
};

DPHX_END
