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
    void SetFirst(RigidBody* body, const Vector3& position)
    {
        _bodies[0] = body;
        _positions[0] = position;
    }

    void SetSecond(RigidBody* body, const Vector3& position)
    {
        _bodies[1] = body;
        _positions[1] = position;
    }

    RigidBody* GetFirst() const { return _bodies[0]; }
    const Vector3& GetFirstPosition() const { return _positions[0]; }
    RigidBody* GetSecond() const { return _bodies[1]; }
    const Vector3& GetSecondPosition() const { return _positions[1]; }

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
