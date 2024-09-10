#pragma once

#include "DgeX/Physics/phxpch.h"

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

    /**
     * @brief Set the mass of the particle.
     * @param mass the original mass of the object
     * @warning mass can't be zero!
     */
    void SetMass(real_t mass);
    real_t GetMass() const;

    /**
     * @brief Set the inverse mass of the particle.
     * @param inverseMass the inverse mass, 0 for infinite mass
     */
    void SetInverseMass(real_t inverseMass) { _inverseMass = inverseMass; }
    real_t GetInverseMass() const { return _inverseMass; }
    void SetInfiniteMass() { SetInverseMass(0); }
    bool HasFiniteMass() const { return _inverseMass > 0; }
    bool HasInfiniteMass() const { return !HasFiniteMass(); }

    void SetDamping(real_t damping) { _damping = damping; }
    real_t GetDamping() const { return _damping; }

    void SetVelocity(const Vector3& velocity) { _velocity = velocity; }
    const Vector3& GetVelocity() const { return _velocity; }

    void SetAcceleration(const Vector3& acceleration) { _acceleration = acceleration; }
    const Vector3& GetAcceleration() const { return _acceleration; }

    void ApplyForce(const Vector3& force);
    void ClearAccumulatedForce() { _accumulatedForce.Clear(); }

protected:
    /**
     * @brief
     * We use inverse mass instead of mass because mass is always used in the
     * denominator in physics calculations. This way we can avoid division by
     * zero and use zero as a special value to represent infinite mass.
     */
    real_t _inverseMass = 0.0;

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
     * The inherent acceleration of the particle. The particle always has this
     * acceleration, e.g. gravity. The outside force will be added to this.
     *
     * To simulate more complex force, we can set it to 0.0 (no acceleration) and
     * then use a force generator to apply acceleration force.
     */
    Vector3 _acceleration;

    /**
     * @brief
     * Accumulated force in the current simulation step. This should be reset
     * when the simulation step completes.
     */
    Vector3 _accumulatedForce;

    /**
     * @brief
     * The current velocity of the particle. This is the only state (other than
     * position) that maintains during the simulation.
     */
    Vector3 _velocity;
};

// clang-format on

DPHX_END
