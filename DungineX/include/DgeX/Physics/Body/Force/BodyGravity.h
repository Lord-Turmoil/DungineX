#pragma once

#include "DgeX/Physics/Body/Force/BodyForceGenerator.h"
#include "DgeX/Physics/Core/Types/Vector.h"

DPHX_BEGIN

class BodyGravity : public BodyForceGenerator
{
public:
    BodyGravity() = default;

    BodyGravity(const Vector3& gravity) : Gravity(gravity)
    {
    }

    void UpdateForce(RigidBody* body, real_t delta) override;

public:
    Vector3 Gravity;
};

DPHX_END
