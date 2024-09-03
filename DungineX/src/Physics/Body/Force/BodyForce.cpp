#include "DgeX/Physics/Body/Force/BodyForce.h"

#include "DgeX/Physics/Body/RigidBody.h"

DPHX_BEGIN

void BodyForce::UpdateForce(RigidBody* body, real_t /*delta*/)
{
    if (body->HasFiniteMass())
    {
        body->ApplyForce(Force);
    }
}

DPHX_END
