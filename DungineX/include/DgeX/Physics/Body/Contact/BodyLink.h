#pragma once

#include "DgeX/Physics/Body/Contact/BodyContactGenerator.h"
#include "DgeX/Physics/Core/Types/Vector.h"

DPHX_BEGIN

class RigidBody;

/**
 * @brief Generate contact between two rigid bodies.
 */
class BodyLink : public BodyContactGenerator
{
public:
    BodyLink() : _bodies{ nullptr, nullptr }
    {
    }

    ~BodyLink() override = default;

public:
    // clang-format off
    void First(RigidBody* body, const Vector3& position)
    {
        _bodies[0] = body;
        _positions[0] = position;
    }
    RigidBody* First() const { return _bodies[0]; }
    const Vector3& FirstPosition() const { return _positions[0]; }

    void Second(RigidBody* body, const Vector3& position)
    {
        _bodies[1] = body;
        _positions[1] = position;
    }
    RigidBody* Second() const { return _bodies[1]; }
    const Vector3& SecondPosition() const { return _positions[1]; }

    // clang-format on

protected:
    /**
     * @brief The two bodies that are connected.
     */
    RigidBody* _bodies[2];

    /**
     * @brief The local position of the connection in two bodies.
     */
    Vector3 _positions[2];
};

DPHX_END
