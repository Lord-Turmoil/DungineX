#pragma once

#include "DgeX/Physics/Common.h"

DPHX_BEGIN

// clang-format off
/**
 * @brief
 * Particle represents a point mass in space. It only has mass, and doesn't
 * have collision shape.
 */
class Particle : public DObject
{
public:
    ~Particle() override = default;

    void Integrate(real_t delta) override;

    void SetDamping(real_t damping) { _damping = damping; }
    real_t GetDamping() const { return _damping; }

    void ApplyForce(const Vector3& force);
    void ClearAccumulators() { _accumulatedForce.Clear(); }

protected:
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
