#pragma once

#include "DgeX/Physics/Core/Types.h"

DPHX_BEGIN

// clang-format off
/**
 * @brief
 * This is the base class for all objects in the physics engine, to be specific,
 * Particle and RigidBody.
 * We assume that every object has a position in the world.
 */
class DObject
{
public:
    DObject() = default;

    DObject(const Vector3& position) : _position(position)
    {
    }

    virtual ~DObject() = default;

    /**
     * @brief
     * Do the integration for the object. This is the main function that
     * update the object's state in the physics simulation.
     * @param delta elapsed time since the last step
     */
    virtual void Integrate(real_t delta) = 0;

    /**
     * @brief Get the position of the object in the world.
     * @return the position
     */
    const Vector3& GetPosition() const { return _position; }

    /**
     * @brief Set the position of the object in the world.
     * @param position the position in the world
     */
    void SetPosition(const Vector3& position) { _position = position; }
    void SetPosition(real_t x, real_t y, real_t z) { _position.Set(x, y, z); }

    /**
     * @brief Set the 2D position in the world, Z is set to 0.
     * @param x X position
     * @param y Y position
     */
    void SetPosition(real_t x, real_t y) { SetPosition(x, y, 0); }

    /**
     * @brief Translate the object by the given translation vector.
     * @param translation the translation
     */
    void Translate(const Vector3& translation) { _position += translation; }
    void Translate(real_t x, real_t y, real_t z) { _position.AddVector(x, y, z); }

    /**
     * @brief Translate the object in the 2D world.
     * @param x translation in X
     * @param y translation in Y
     */
    void Translate(real_t x, real_t y) { Translate(x, y, 0); }

    /**
     * @brief Set the mass of the particle.
     * @param mass the original mass of the object
     * @warning mass can't be zero!
     */
    void SetMass(real_t mass);
    real_t GetMass() const;

    void SetInverseMass(real_t inverseMass) { _inverseMass = inverseMass; }
    real_t GetInverseMass() const { return _inverseMass; }
    void SetInfiniteMass() { SetInverseMass(0); }
    bool HasFiniteMass() const { return _inverseMass > 0; }
    bool HasInfiniteMass() const { return !HasFiniteMass(); }

    void SetAcceleration(const Vector3& acceleration) { _acceleration = acceleration; }
    void AddAcceleration(const Vector3& acceleration) { _acceleration += acceleration; }
    const Vector3& GetAcceleration() const { return _acceleration; }

    void SetVelocity(const Vector3& velocity) { _velocity = velocity; }
    void AddVelocity(const Vector3& velocity) { _velocity += velocity; }
    const Vector3& GetVelocity() const { return _velocity; }

protected:
    /**
     * @brief The position of the object in the 3D world.
     */
    Vector3 _position;

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
     * The current velocity of the particle. This is the only state (other than
     * position) that maintains during the simulation.
     */
    Vector3 _velocity;

    /**
     * @brief
     * We use inverse mass instead of mass because mass is always used in the
     * denominator in physics calculations. This way we can avoid division by
     * zero and use zero as a special value to represent infinite mass.
     */
    real_t _inverseMass = 0.0;
};

// clang-format on

DPHX_END
