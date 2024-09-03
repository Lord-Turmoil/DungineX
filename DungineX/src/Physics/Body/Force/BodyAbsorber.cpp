#include "DgeX/Physics/Body/Force/BodyAbsorber.h"

#include "DgeX/Physics/Body/RigidBody.h"

#include "DgeX/Core/Assert.h"

DPHX_BEGIN

void BodyAbsorber::UpdateForce(RigidBody* body, real_t /*delta*/)
{
    DGEX_ASSERT(Other);

    // Get the vector in world space.
    Vector3 thisPoint = body->ToWorldPoint(ConnectionPoint);
    Vector3 otherPoint = Other->ToWorldPoint(OtherConnectionPoint);
    Vector3 force = thisPoint - otherPoint;

    real_t springForce = force.Magnitude();
    springForce = (RestLength - springForce) * SpringConstant;

    force.Normalize();
    real_t dampingForce = (body->GetVelocity() - Other->GetVelocity()) * force * Damping;

    force *= (springForce - dampingForce);

    body->ApplyForceAtWorldPoint(force, thisPoint);
}

DPHX_END
