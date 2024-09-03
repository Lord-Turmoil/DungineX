#pragma once

#include "DgeX/Physics/Core/Macros.h"
#include "DgeX/Physics/Core/Precision.h"

DPHX_BEGIN

class ParticleContact;

/**
 * @brief Responsible for resolving contacts between particles.
 * @note Not ready for inheritance.
 */
class ParticleContactResolver final
{
public:
    ParticleContactResolver(uint32_t iterations);
    ~ParticleContactResolver() = default;

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

private:
    /**
     * @brief
     * The number of iterations allowed for the resolution of contacts.
     */
    uint32_t _iterations;
};

DPHX_END
