#pragma once

#include "DgeX/Physics/Core/Types/Vector.h"
#include "DgeX/Physics/Particle/Contact/ParticleContactGenerator.h"

DPHX_BEGIN

class Particle;

/**
 * @brief Base class for anchored particle link.
 * @note Used to generate contact between particle and a fixed point.
 */
class ParticleAnchoredLink : public ParticleContactGenerator
{
public:
    ParticleAnchoredLink() : _particle(nullptr), _anchor(nullptr)
    {
    }

    ParticleAnchoredLink(Particle* particle, Vector3* anchor) : _particle(particle), _anchor(anchor)
    {
    }

    ~ParticleAnchoredLink() override = default;

    // clang-format off
    void SetParticle(Particle* particle) { _particle = particle; }
    Particle* GetParticle() const { return _particle; }

    void SetAnchor(Vector3* anchor) { _anchor = anchor; }
    Vector3* GetAnchor() const { return _anchor; }

    // clang-format on
protected:
    real_t _Length() const;

protected:
    Particle* _particle;
    Vector3* _anchor;
};

/**
 * @brief Base class for particle link.
 * @note Generate contact between two contacts.
 */
class ParticleLink : public ParticleContactGenerator
{
public:
    ParticleLink() : _particles{ nullptr, nullptr }
    {
    }

    ParticleLink(Particle* first, Particle* second) : _particles{ first, second }
    {
    }

    ~ParticleLink() override = default;

    // clang-format off
    void SetFirst(Particle* particle) { _particles[0] = particle; }
    Particle* GetFirst() const { return _particles[0]; }

    void SetSecond(Particle* particle) { _particles[1] = particle; }
    Particle* GetSecond() const { return _particles[1]; }

    // clang-format on
protected:
    real_t _Length() const;

protected:
    Particle* _particles[2];
};

DPHX_END
