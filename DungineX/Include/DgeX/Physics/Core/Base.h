#pragma once

#include "DgeX/Physics/Core/Types.h"

DPHX_BEGIN

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
     * @param deltaTime elapsed time since the last step
     */
    virtual void Integrate(real_t deltaTime) = 0;

    /**
     * @brief Get the position of the object in the world.
     * @return the position
     */
    const Vector3& GetPosition() const
    {
        return _position;
    }

    /**
     * @brief Set the position of the object in the world.
     * @param position the position in the world
     */
    void SetPosition(const Vector3& position)
    {
        _position = position;
    }

    void SetPosition(real_t x, real_t y, real_t z)
    {
        _position.X = x;
        _position.Y = y;
        _position.Z = z;
    }

    /**
     * @brief Set the 2D position in the world, Z is set to 0.
     * @param x X position
     * @param y Y position
     */
    void SetPosition(real_t x, real_t y)
    {
        SetPosition(x, y, 0);
    }

    /**
     * @brief Translate the object by the given translation vector.
     * @param translation the translation
     */
    void Translate(const Vector3& translation)
    {
        _position += translation;
    }

    void Translate(real_t x, real_t y, real_t z)
    {
        _position.X += x;
        _position.Y += y;
        _position.Z += z;
    }

    /**
     * @brief Translate the object in the 2D world.
     * @param x translation in X
     * @param y translation in Y
     */
    void Translate(real_t x, real_t y)
    {
        Translate(x, y, 0);
    }

protected:
    Vector3 _position;
};

DPHX_END
