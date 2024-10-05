#pragma once

#include "DgeX/Physics/Common.h"
#include "DgeX/Physics/Core/Types/Vector.h"

DPHX_BEGIN

class Particle;

class ParticleContact
{
    friend class ParticleContactResolver;

public:
    virtual ~ParticleContact() = default;

protected:
    void _Resolve(real_t delta);
    real_t _CalculateSeparatingVelocity() const;

private:
    void _ResolveVelocity(real_t delta) const;
    void _ResolveInterpenetration(real_t delta);

public:
    // clang-format off
    void SetFirst(Particle* particle) { _particles[0] = particle; }
    Particle* GetFirst() const { return _particles[0]; }

    void SetSecond(Particle* particle) { _particles[1] = particle; }
    Particle* GetSecond() const { return _particles[1]; }

    // clang-format on

public:
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

DPHX_END
