#include "DgeX/Physics/Particle/Contact/ParticleRod.h"
#include "DgeX/Physics/Particle/Contact/ParticleContact.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

uint32_t ParticleAnchoredRod::AddContact(ParticleContact* contact, uint32_t /*limit*/) const
{
    real_t length = _Length();
    if (length == Length)
    {
        return 0;
    }

    contact->SetFirst(_particle);
    contact->SetSecond(nullptr);

    Vector3 normal = *_anchor - _particle->GetPosition();
    normal.Normalize();

    if (length > Length)
    {
        contact->ContactNormal = normal;
        contact->Penetration = length - Length;
    }
    else
    {
        contact->ContactNormal = -normal;
        contact->Penetration = Length - length;
    }
    contact->Restitution = 0.0;

    return 1;
}

uint32_t ParticleRod::AddContact(ParticleContact* contact, uint32_t /*limit*/) const
{
    real_t length = _Length();
    if (length == Length)
    {
        return 0;
    }

    contact->SetFirst(_particles[0]);
    contact->SetSecond(_particles[1]);

    // Normal points to the direction that particle[0] should move.
    Vector3 normal = _particles[1]->GetPosition() - _particles[0]->GetPosition();
    normal.Normalize();

    if (length > Length)
    {
        contact->ContactNormal = normal;
        contact->Penetration = length - Length;
    }
    else
    {
        contact->ContactNormal = -normal;
        contact->Penetration = Length - length;
    }

    contact->Restitution = 0.0;

    return 1;
}

DPHX_END
