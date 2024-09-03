#pragma once

#include "DgeX/Physics/Particle/Contact/ParticleLink.h"

DPHX_BEGIN

/**
 * @brief
 * A panel that restricts the movement of a particle to a certain plane.
 * It can be used as ground.
 */
class ParticlePanel : public ParticleContactGenerator
{
public:
    ParticlePanel() : Particle(nullptr), Normal(Vector3::UnitY), Anchor(Vector3::Zero), Restitution(0.2f)
    {
    }

    ParticlePanel(Particle* particle, const Vector3& normal, const Vector3& anchor, real_t restitution = 0.2f)
        : Particle(particle), Normal(normal), Anchor(anchor), Restitution(restitution)
    {
        Normal.Normalize(); // ensure we have a valid normal
    }

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

public:
    Particle* Particle;

    Vector3 Normal;
    Vector3 Anchor;

    real_t Restitution;
};

/**
 * @brief Apply panel constraint to multiple particles.
 */
class ParticlesPanel : public ParticleContactGenerator
{
public:
    ParticlesPanel() : Normal(Vector3::UnitY), Restitution(0.1f), _particles(nullptr), _count(0)
    {
    }

    ParticlesPanel(Particle* particles, size_t count, const Vector3& normal, const Vector3& anchor,
                   real_t restitution = 0.1f)
        : Normal(normal), Anchor(anchor), Restitution(restitution), _particles(particles), _count(count)
    {
    }

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetParticles(Particle* particles, size_t count) { _particles = particles; _count = count; }
    Particle* GetParticles() const { return _particles; }
    size_t GetCount() const { return _count; }

    // clang-format on
public:
    Vector3 Normal;
    Vector3 Anchor;

    real_t Restitution;

protected:
    Particle* _particles;
    size_t _count;
};

DPHX_END
