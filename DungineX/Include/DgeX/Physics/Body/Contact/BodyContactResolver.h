#pragma once

#include "DgeX/Physics/Common.h"
#include "DgeX/Physics/Core/Precision.h"

DPHX_BEGIN

class BodyContact;

/**
 * @brief Responsible for resolving contacts between rigid bodies.
 *
 * @paragraph algorithm
 *
 * This resolver solves contact locally, which means solving one contact can
 * make other contacts worse. To solve this, the resolver will iterate through
 * all contacts multiple times to best approximate the global solution.
 *
 * @paragraph limitation
 *
 * This algorithm does not perform well for inter-related contacts. For example,
 * stacked boxes may jiggle a little, resulting in collapse.
 *
 * Also, resolving contacts may cause object translate regardless of friction.
 */
class BodyContactResolver
{
public:
    BodyContactResolver(uint32_t iterations);
    virtual ~BodyContactResolver() = default;

    // clang-format off
    void SetVelocityIterations(uint32_t iterations) { _velocityIterations = iterations; }
    void SetPositionIterations(uint32_t iterations) { _positionIterations = iterations; }

    // epsilon should be greater than 0
    void SetVelocityEpsilon(real_t epsilon) { _velocityEpsilon = epsilon; }
    // epsilon should be greater than 0
    void SetPositionEpsilon(real_t epsilon) { _positionEpsilon = epsilon; }

    // clang-format on

    void Resolve(BodyContact* contacts, uint32_t count, real_t delta);

protected:
    /**
     * @brief
     * Prepare contacts' data and set their awake state.
     */
    void _PrepareContacts(BodyContact* contacts, uint32_t count, real_t delta);

    /**
     * @brief Resolve velocity constraints.
     */
    void _AdjustVelocities(BodyContact* contacts, uint32_t count, real_t delta) const;

    /**
     * @brief Resolve position constraints.
     */
    void _AdjustPositions(BodyContact* contacts, uint32_t count, real_t delta) const;

protected:
    /**
     * @brief Iteration 0 will use the 2 * count as the number of iterations.
     */
    uint32_t _velocityIterations = 0;
    uint32_t _positionIterations = 0;

    /**
     * @brief
     * Taking velocity smaller than this value as zero to avoid instability.
     */
    real_t _velocityEpsilon = 0.01;

    /**
     * @brief
     * Taking position smaller than this value as zero to avoid instability.
     */
    real_t _positionEpsilon = 0.01;
};

DPHX_END
