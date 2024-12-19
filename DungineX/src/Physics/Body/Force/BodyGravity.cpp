#include "DgeX/Physics/Body/Force/BodyGravity.h"
#include "DgeX/Physics/Body/RigidBody.h"

DPHX_BEGIN

void BodyGravity::UpdateForce(RigidBody* body, real_t /*delta*/)
{
    if (body->HasFiniteMass())
    {
        body->ApplyForce(Gravity * body->GetMass());
    }
}

DPHX_END
