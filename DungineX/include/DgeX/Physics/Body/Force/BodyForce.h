#pragma once

#include "DgeX/Physics/Body/Force/BodyForceGenerator.h"
#include "DgeX/Physics/Core/Types/Vector.h"

DPHX_BEGIN

class BodyForce : public BodyForceGenerator
{
public:
    BodyForce() = default;

    BodyForce(const Vector3& force) : Force(force)
    {
    }

    void UpdateForce(RigidBody* body, real_t delta) override;

public:
    Vector3 Force;
};

DPHX_END
