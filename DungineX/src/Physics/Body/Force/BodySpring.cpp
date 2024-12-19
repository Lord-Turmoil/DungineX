#include "DgeX/Physics/Body/Force/BodySpring.h"

#include "DgeX/Physics/Body/RigidBody.h"

#include "DgeX/Common/Assert.h"

DPHX_BEGIN

void BodyAnchoredSpring::UpdateForce(RigidBody* body, real_t /*delta*/)
{
    DGEX_ASSERT(Anchor);

    Vector3 thisPoint = body->ToWorldPoint(ConnectionPoint);
    Vector3 force = thisPoint - *Anchor;

    real_t magnitude = force.Magnitude();
    magnitude = (RestLength - magnitude) * SpringConstant;

    force.Normalize();
    force *= magnitude;

    body->ApplyForceAtWorldPoint(force, thisPoint);
}

void BodySpring::UpdateForce(RigidBody* body, real_t /*delta*/)
{
    DGEX_ASSERT(Other);

    // Get the vector in world space.
    Vector3 thisPoint = body->ToWorldPoint(ConnectionPoint);
    Vector3 otherPoint = Other->ToWorldPoint(OtherConnectionPoint);
    Vector3 force = thisPoint - otherPoint;

    real_t magnitude = force.Magnitude();
    magnitude = (RestLength - magnitude) * SpringConstant;

    force.Normalize();
    force *= magnitude;

    body->ApplyForceAtWorldPoint(force, thisPoint);
}

DPHX_END
