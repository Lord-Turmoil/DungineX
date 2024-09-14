#include "DgeX/Physics/Particle/ParticleLink.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

real_t ParticleAnchoredLink::_Length() const
{
    return (_particle->GetPosition() - *_anchor).Magnitude();
}

int ParticleAnchoredCable::AddContact(ParticleContact* contact, int limit) const
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
    contact->SetContactNormal(normal);

    contact->SetPenetration(length - _maxLength);
    contact->SetRestitution(_restitution);

    return 1;
}

int ParticleAnchoredHalfRod::AddContact(ParticleContact* contact, int limit) const
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
    contact->SetContactNormal(normal);

    contact->SetPenetration(_minLength - length);
    contact->SetRestitution(_restitution);

    return 1;
}

int ParticleAnchoredRod::AddContact(ParticleContact* contact, int limit) const
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
        contact->SetContactNormal(normal);
        contact->SetPenetration(length - _length);
    }
    else
    {
        contact->SetContactNormal(-normal);
        contact->SetPenetration(_length - length);
    }
    contact->SetRestitution(0);

    return 1;
}

real_t ParticleLink::_Length() const
{
    return (_particles[0]->GetPosition() - _particles[1]->GetPosition()).Magnitude();
}

int ParticleCable::AddContact(ParticleContact* contact, int limit) const
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
    contact->SetContactNormal(normal);

    contact->SetPenetration(length - _maxLength);
    contact->SetRestitution(_restitution);

    return 1;
}

int ParticleHalfRod::AddContact(ParticleContact* contact, int limit) const
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
    contact->SetContactNormal(normal);

    contact->SetPenetration(_minLength - length);
    contact->SetRestitution(_restitution);

    return 1;
}

int ParticleRod::AddContact(ParticleContact* contact, int limit) const
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
        contact->SetContactNormal(normal);
        contact->SetPenetration(length - _length);
    }
    else
    {
        contact->SetContactNormal(-normal);
        contact->SetPenetration(_length - length);
    }
    contact->SetRestitution(0);

    return 1;
}

int ParticlePanel::AddContact(ParticleContact* contact, int limit) const
{
    Vector3 position = _particle->GetPosition() - _anchor;
    real_t penetration = -(position * _normal);

    if (penetration <= 0)
    {
        return 0;
    }

    contact->SetFirst(_particle);
    contact->SetSecond(nullptr);
    contact->SetContactNormal(_normal);
    contact->SetPenetration(penetration);
    contact->SetRestitution(_restitution);

    return 1;
}

int ParticlesPanel::AddContact(ParticleContact* contact, int limit) const
{
    int added = 0;

    Particle* end = _particles + _count;
    for (Particle* particle = _particles; (particle != end) && (added < limit); particle++)
    {
        Vector3 position = particle->GetPosition() - _anchor;
        real_t penetration = -(position * _normal);
        if (penetration <= 0)
        {
            continue;
        }

        contact->SetFirst(particle);
        contact->SetSecond(nullptr);
        contact->SetContactNormal(_normal);
        contact->SetPenetration(penetration);
        contact->SetRestitution(_restitution);

        added++;
    }

    return added;
}

DPHX_END
