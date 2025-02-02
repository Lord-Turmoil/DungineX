#include "DgeX/Physics/Body/Contact/BodyStop.h"

#include "DgeX/Core/Assert.h"
#include "DgeX/Physics/Body/Contact/BodyContact.h"
#include "DgeX/Physics/Body/RigidBody.h"

DPHX_BEGIN

uint32_t BodyStop::AddContact(BodyContact* contact, uint32_t /*limit*/) const
{
    DGEX_ASSERT(_bodies[0] && _bodies[1]);

    Vector3 a_pos_world = _bodies[0]->ToWorldPoint(_positions[0]);
    Vector3 b_pos_world = _bodies[1]->ToWorldPoint(_positions[1]);

    Vector3 a_to_b = b_pos_world - a_pos_world;
    Vector3 normal = a_to_b.Normalized();
    double length = a_to_b.Magnitude();

    if (length >= MinLength)
    {
        return 0;
    }

    contact->First(_bodies[0]);
    contact->Second(_bodies[1]);
    contact->ContactPoint = (a_pos_world + b_pos_world) * 0.5f;
    contact->ContactNormal = -normal;
    contact->Penetration = MinLength - length;
    contact->Friction = 1.0;
    contact->Restitution = 0.0;

    return 1;
}

DPHX_END
