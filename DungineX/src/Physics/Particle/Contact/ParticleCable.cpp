#include "DgeX/Physics/Particle/Contact/ParticleCable.h"
#include "DgeX/Physics/Particle/Contact/ParticleContact.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

uint32_t ParticleAnchoredCable::AddContact(ParticleContact* contact, uint32_t /*limit*/) const
{
    real_t length = _Length();
    if (length < MaxLength)
    {
        return 0;
    }

    contact->First(_particle);
    contact->Second(nullptr);

    Vector3 normal = *_anchor - _particle->GetPosition();
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->Penetration = length - MaxLength;
    contact->Restitution = Restitution;

    return 1;
}

uint32_t ParticleCable::AddContact(ParticleContact* contact, uint32_t /*limit*/) const
{
    real_t length = _Length();
    if (length < MaxLength)
    {
        return 0;
    }

    contact->First(_particles[0]);
    contact->Second(_particles[1]);

    // Normal points to the direction that particle[0] should move.
    Vector3 normal = _particles[1]->GetPosition() - _particles[0]->GetPosition();
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->Penetration = length - MaxLength;
    contact->Restitution = Restitution;

    return 1;
}

DPHX_END
