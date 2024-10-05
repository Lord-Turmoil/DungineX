#pragma once

#include "DgeX/Physics/Body/Force/BodyForceGenerator.h"
#include "DgeX/Physics/Core/Types/Vector.h"

DPHX_BEGIN

class BodyAnchoredSpring : public BodyForceGenerator
{
public:
    BodyAnchoredSpring() : Anchor(nullptr), SpringConstant(0), RestLength(0)
    {
    }

    BodyAnchoredSpring(Vector3* anchor, real_t springConstant, real_t restLength)
        : Anchor(anchor), SpringConstant(springConstant), RestLength(restLength)
    {
    }

    ~BodyAnchoredSpring() override = default;

    void UpdateForce(RigidBody* body, real_t delta) override;

public:
    /**
     * @brief Anchor point in world space.
     */
    Vector3* Anchor;

    /**
     * @brief Connection point of this body in local space.
     */
    Vector3 ConnectionPoint;

    real_t SpringConstant;
    real_t RestLength;
};

class BodySpring : public BodyForceGenerator
{
public:
    BodySpring() : Other(nullptr), SpringConstant(0), RestLength(0)
    {
    }

    BodySpring(RigidBody* other, real_t springConstant, real_t restLength)
        : Other(other), SpringConstant(springConstant), RestLength(restLength)
    {
    }

    ~BodySpring() override = default;

    void UpdateForce(RigidBody* body, real_t delta) override;

public:
    RigidBody* Other;

    /**
     * @brief Connection point on this body in local space.
     */
    Vector3 ConnectionPoint;

    /**
     * @brief Connection point of other body in its local space.
     */
    Vector3 OtherConnectionPoint;

    real_t SpringConstant;
    real_t RestLength;
};

DPHX_END
