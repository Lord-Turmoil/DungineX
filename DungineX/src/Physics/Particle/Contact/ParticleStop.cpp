#include "DgeX/Physics/Particle/Contact/ParticleStop.h"
#include "DgeX/Physics/Particle/Contact/ParticleContact.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

uint32_t ParticleAnchoredStop::AddContact(ParticleContact* contact, uint32_t /*limit*/) const
{
    real_t length = _Length();
    if (length > MinLength)
    {
        return 0;
    }

    contact->SetFirst(_particle);
    contact->SetSecond(nullptr);

    Vector3 normal = _particle->GetPosition() - *_anchor;
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->Penetration = MinLength - length;
    contact->Restitution = Restitution;

    return 1;
}

uint32_t ParticleStop::AddContact(ParticleContact* contact, uint32_t /*limit*/) const
{
    real_t length = _Length();
    if (length > MinLength)
    {
        return 0;
    }

    contact->SetFirst(_particles[0]);
    contact->SetSecond(_particles[1]);

    // Normal points to the direction that particle[0] should move.
    Vector3 normal = _particles[0]->GetPosition() - _particles[1]->GetPosition();
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->Penetration = MinLength - length;
    contact->Restitution = Restitution;

    return 1;
}

DPHX_END
