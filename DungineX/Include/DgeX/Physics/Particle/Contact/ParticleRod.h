#pragma once

#include "DgeX/Physics/Particle/Contact/ParticleLink.h"

DPHX_BEGIN

/**
 * @brief Restricts the distance between two particles.
 */
class ParticleAnchoredRod : public ParticleAnchoredLink
{
public:
    ParticleAnchoredRod() : Length(0)
    {
    }

    ParticleAnchoredRod(Particle* particle, Vector3* anchor, real_t length)
        : ParticleAnchoredLink(particle, anchor), Length(length)
    {
    }

    ~ParticleAnchoredRod() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

public:
    real_t Length;
};

/**
 * @brief Restricts the distance between two particles.
 */
class ParticleRod : public ParticleLink
{
public:
    ParticleRod() : Length(0)
    {
    }

    ParticleRod(Particle* first, Particle* second, real_t length) : ParticleLink(first, second), Length(length)
    {
    }

    ~ParticleRod() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

public:
    real_t Length;
};

DPHX_END
