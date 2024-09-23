#include "DgeX/Physics/Particle/ParticleLink.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

real_t ParticleAnchoredLink::_Length() const
{
    return (_particle->GetPosition() - *_anchor).Magnitude();
}

uint32_t ParticleAnchoredCable::AddContact(ParticleContact* contact, uint32_t limit) const
{
    real_t length = _Length();
    if (length < _maxLength)
    {
        return 0;
    }

    contact->SetFirst(_particle);
    contact->SetSecond(nullptr);

    Vector3 normal = *_anchor - _particle->GetPosition();
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->Penetration = length - _maxLength;
    contact->Restitution = _restitution;

    return 1;
}

uint32_t ParticleAnchoredHalfRod::AddContact(ParticleContact* contact, uint32_t limit) const
{
    real_t length = _Length();
    if (length > _minLength)
    {
        return 0;
    }

    contact->SetFirst(_particle);
    contact->SetSecond(nullptr);

    Vector3 normal = _particle->GetPosition() - *_anchor;
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->Penetration = _minLength - length;
    contact->Restitution = _restitution;

    return 1;
}

uint32_t ParticleAnchoredRod::AddContact(ParticleContact* contact, uint32_t limit) const
{
    real_t length = _Length();
    if (length == _length)
    {
        return 0;
    }

    contact->SetFirst(_particle);
    contact->SetSecond(nullptr);

    Vector3 normal = *_anchor - _particle->GetPosition();
    normal.Normalize();

    if (length > _length)
    {
        contact->ContactNormal = normal;
        contact->Penetration = length - _length;
    }
    else
    {
        contact->ContactNormal = -normal;
        contact->Penetration = _length - length;
    }

    contact->Restitution = 0.0;

    return 1;
}

real_t ParticleLink::_Length() const
{
    return (_particles[0]->GetPosition() - _particles[1]->GetPosition()).Magnitude();
}

uint32_t ParticleCable::AddContact(ParticleContact* contact, uint32_t limit) const
{
    real_t length = _Length();
    if (length < _maxLength)
    {
        return 0;
    }

    contact->SetFirst(_particles[0]);
    contact->SetSecond(_particles[1]);

    // Normal points to the direction that particle[0] should move.
    Vector3 normal = _particles[1]->GetPosition() - _particles[0]->GetPosition();
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->Penetration = length - _maxLength;
    contact->Restitution = _restitution;

    return 1;
}

uint32_t ParticleHalfRod::AddContact(ParticleContact* contact, uint32_t limit) const
{
    real_t length = _Length();
    if (length > _minLength)
    {
        return 0;
    }

    contact->SetFirst(_particles[0]);
    contact->SetSecond(_particles[1]);

    // Normal points to the direction that particle[0] should move.
    Vector3 normal = _particles[0]->GetPosition() - _particles[1]->GetPosition();
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->Penetration = _minLength - length;
    contact->Restitution = _restitution;

    return 1;
}

uint32_t ParticleRod::AddContact(ParticleContact* contact, uint32_t limit) const
{
    real_t length = _Length();
    if (length == _length)
    {
        return 0;
    }

    contact->SetFirst(_particles[0]);
    contact->SetSecond(_particles[1]);

    // Normal points to the direction that particle[0] should move.
    Vector3 normal = _particles[1]->GetPosition() - _particles[0]->GetPosition();
    normal.Normalize();

    if (length > _length)
    {
        contact->ContactNormal = normal;
        contact->Penetration = length - _length;
    }
    else
    {
        contact->ContactNormal = -normal;
        contact->Penetration = _length - length;
    }

    contact->Restitution = 0.0;

    return 1;
}

uint32_t ParticlePanel::AddContact(ParticleContact* contact, uint32_t limit) const
{
    Vector3 position = _particle->GetPosition() - _anchor;
    real_t penetration = -(position * _normal);

    if (penetration <= 0)
    {
        return 0;
    }

    contact->SetFirst(_particle);
    contact->SetSecond(nullptr);
    contact->ContactNormal = _normal;
    contact->Penetration = penetration;
    contact->Restitution = _restitution;

    return 1;
}

uint32_t ParticlesPanel::AddContact(ParticleContact* contact, uint32_t limit) const
{
    uint32_t added = 0;

    Particle* end = _particles + _count;
    for (Particle* particle = _particles; particle != end && added <= limit; particle++)
    {
        Vector3 position = particle->GetPosition() - _anchor;
        real_t penetration = -(position * _normal);
        if (penetration <= 0)
        {
            continue;
        }

        contact->SetFirst(particle);
        contact->SetSecond(nullptr);
        contact->ContactNormal = _normal;
        contact->Penetration = penetration;
        contact->Restitution = _restitution;

        contact++;
        added++;
    }

    return added;
}

DPHX_END
