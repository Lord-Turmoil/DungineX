#pragma once

#include "DgeX/Physics/Body/Force/BodySpring.h"

DPHX_BEGIN

class BodyAbsorber : public BodySpring
{
public:
    BodyAbsorber() : Damping(0)
    {
    }

    BodyAbsorber(RigidBody* other, real_t springConstant, real_t restLength, real_t damping)
        : BodySpring(other, springConstant, restLength), Damping(damping)
    {
    }

    ~BodyAbsorber() override = default;

    void UpdateForce(RigidBody* body, real_t delta) override;

public:
    real_t Damping;
};

DPHX_END
