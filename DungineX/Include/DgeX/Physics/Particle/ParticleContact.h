#pragma once

#include "DgeX/Physics/phxpch.h"

DPHX_BEGIN

class Particle;

class ParticleContact
{
    friend class ParticleContactResolver;

public:
    virtual ~ParticleContact() = default;

    // clang-format off
    void SetFirst(Particle* particle) { _particles[0] = particle; }
    Particle* GetFirst() const { return _particles[0]; }

    void SetSecond(Particle* particle) { _particles[1] = particle; }
    Particle* GetSecond() const { return _particles[1]; }

    // clang-format on

    /**
     * @brief The direction of the contact in world coordinates.
     * @note
     * It is calculated based on the first particle, and points to the direction
     * that separate the two particles.
     */
    Vector3 ContactNormal;

    /**
     * @brief The depth of penetration at the contact.
     */
    real_t Penetration = 0;

    /**
     * @brief
     * The normal restitution coefficient at the contact.
     * 1.0 means the contact is perfectly elastic, 0.0 means no bounce and the
     * two particles will stick together.
     */
    real_t Restitution = 0;

protected:
    void _Resolve(real_t delta);
    real_t _CalculateSeparatingVelocity() const;

private:
    void _ResolveVelocity(real_t delta) const;
    void _ResolveInterpenetration(real_t delta);

protected:
    /**
     * @brief
     * The two particles involved in the contact. If it is contact with
     * the scenery, the second particle is set to nullptr.
     */
    Particle* _particles[2];

    /**
     * @brief Movement of the particles during the resolution.
     */
    Vector3 _movements[2];
};

/**
 * @brief Responsible for resolving contacts between particles.
 */
class ParticleContactResolver
{
public:
    ParticleContactResolver(uint32_t iterations);
    virtual ~ParticleContactResolver() = default;

    void SetIterations(uint32_t iterations)
    {
        _iterations = iterations;
    }

    /**
     * @brief
     * Resolve a set of particle contacts for both penetration and velocity.
     * Use raw array for compatibility.
     */
    void Resolve(ParticleContact* contacts, uint32_t count, real_t delta) const;

protected:
    /**
     * @brief
     * The number of iterations allowed for the resolution of contacts.
     */
    uint32_t _iterations;
};

/**
 * @brief
 * Base class for contact generators that generate contacts between particles.
 */
class ParticleContactGenerator
{
public:
    virtual ~ParticleContactGenerator() = default;

    /**
     * @brief
     * Add contacts to the contact list with the given limit.
     * The contact should be a pointer in an array of contacts.
     *
     * @note
     * The limit is the maximum number of contacts that can be written, it
     * is greater than or equal to 1.
     */
    virtual uint32_t AddContact(ParticleContact* contact, uint32_t limit) const = 0;
};

/**
 * @brief A registry to hold all particle contact generators.
 */
class ParticleContactRegistry
{
public:
    void Add(ParticleContactGenerator* contactGenerator);
    void Remove(ParticleContactGenerator* contactGenerator);
    void Clear();

    uint32_t AddContact(ParticleContact* contacts, uint32_t limit) const;

private:
    std::vector<ParticleContactGenerator*> _contactGenerators;
};

DPHX_END
