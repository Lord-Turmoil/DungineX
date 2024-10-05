#include "DgeX/Physics/Body/Force/BodyBungee.h"
#include "DgeX/Physics/Body/RigidBody.h"

DPHX_BEGIN

void BodyAnchoredBungee::UpdateForce(RigidBody* body, real_t delta)
{
    DGEX_ASSERT(Anchor);

    Vector3 thisPoint = body->ToWorldPoint(ConnectionPoint);
    Vector3 force = thisPoint - *Anchor;

    real_t magnitude = force.Magnitude();
    if (magnitude < RestLength)
    {
        return;
    }
    magnitude = (RestLength - magnitude) * SpringConstant;

    force.Normalize();
    force *= magnitude;

    body->ApplyForceAtWorldPoint(force, thisPoint);
}

void BodyBungee::UpdateForce(RigidBody* body, real_t delta)
{
    DGEX_ASSERT(Other);

    // Get the vector in world space.
    Vector3 thisPoint = body->ToWorldPoint(ConnectionPoint);
    Vector3 otherPoint = Other->ToWorldPoint(OtherConnectionPoint);
    Vector3 force = thisPoint - otherPoint;

    real_t magnitude = force.Magnitude();
    if (magnitude < RestLength)
    {
        return;
    }
    magnitude = (RestLength - magnitude) * SpringConstant;

    force.Normalize();
    force *= magnitude;

    body->ApplyForceAtWorldPoint(force, thisPoint);
}

DPHX_END
