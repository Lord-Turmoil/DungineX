#pragma once

#include "DgeX/Physics/Core/Base.h"

DPHX_BEGIN

/**
 * @brief
 * Particle represents a point mass in space. It only has mass, and doesn't
 * have collision shape.
 */
// clang-format off
class Particle : public MassObject
{
public:
    ~Particle() override = default;

    void Integrate(real_t delta) override;

    /**
     * @brief
     * Damping is the built-in resistance to the motion of the particle.
     * Although we have the more advanced force generator solution for the
     * resistance, we still keep this simple damping factor for the particle
     * for simplicity.
     *
     * To simulate more complex damping, we can set it to 1.0 (no damping) and
     * then use a force generator to apply damping force.
     */
    void SetDamping(real_t damping) { _damping = damping; }
    real_t GetDamping() const { return _damping; }

    /**
     * @brief Apply a force to the particle.
     * @param force The force.
     */
    void ApplyForce(const Vector3& force);

    /**
     * @brief Clear all the accumulated force in the current simulation step.
     *      That is to say, the particle will not have any force afterward.
     */
    void ClearAccumulators() { _accumulatedForce.Clear(); }

protected:
    
    real_t _damping = 1.0;

    /**
     * @brief
     * Accumulated force in the current simulation step. This should be reset
     * when the simulation step completes.
     */
    Vector3 _accumulatedForce;
};

// clang-format on

DPHX_END
