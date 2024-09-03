#pragma once

#include "DgeX/Physics/Body/Force/BodySpring.h"

DPHX_BEGIN

class BodyAnchoredBungee : public BodyAnchoredSpring
{
public:
    BodyAnchoredBungee() = default;

    BodyAnchoredBungee(Vector3* anchor, real_t springConstant, real_t restLength)
        : BodyAnchoredSpring(anchor, springConstant, restLength)
    {
    }

    ~BodyAnchoredBungee() override = default;

    void UpdateForce(RigidBody* body, real_t delta) override;
};

class BodyBungee : public BodySpring
{
public:
    BodyBungee() = default;

    BodyBungee(RigidBody* other, real_t springConstant, real_t restLength)
        : BodySpring(other, springConstant, restLength)
    {
    }

    ~BodyBungee() override = default;

    void UpdateForce(RigidBody* body, real_t delta) override;
};

DPHX_END
