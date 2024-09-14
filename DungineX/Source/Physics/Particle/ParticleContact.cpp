#include "DgeX/Physics/Particle/ParticleContact.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

void ParticleContact::_Resolve(real_t delta)
{
    _ResolveVelocity(delta);
    _ResolveInterpenetration(delta);
}

real_t ParticleContact::_CalculateSeparatingVelocity() const
{
    Vector3 relativeVelocity = _particles[0]->GetVelocity();
    if (_particles[1])
    {
        relativeVelocity -= _particles[1]->GetVelocity();
    }
    return relativeVelocity * _contactNormal;
}

void ParticleContact::_ResolveVelocity(real_t delta) const
{
    // Apply the impulse to the particles based on their mass.
    // If the particles have infinite mass, they are static and no need to continue.
    real_t totalInverseMass = _particles[0]->GetInverseMass();
    if (_particles[1])
    {
        totalInverseMass += _particles[1]->GetInverseMass();
    }
    if (totalInverseMass <= 0)
    {
        // Contact between... two static objects?
        return;
    }

    // Get the separating velocity.
    real_t separatingVelocity = _CalculateSeparatingVelocity();
    if (separatingVelocity > 0)
    {
        // If separating velocity is greater than zero, the particles
        // are moving away from each other and no impulse is needed.
        return;
    }
    // Correct the separating velocity with the restitution coefficient.
    real_t correctedSeparatingVelocity = -separatingVelocity * _restitution;

    // Acceleration may cause static objects to have a closing velocity which
    // may cause the particles to penetrate each other.
    Vector3 accCausedVelocity = _particles[0]->GetAcceleration();
    if (_particles[1])
    {
        accCausedVelocity -= _particles[1]->GetAcceleration();
    }
    real_t accCausedSepVelocity = accCausedVelocity * _contactNormal * delta;
    // Avoid the velocity due to acceleration so that the particles
    // can be more 'static'.
    if (accCausedSepVelocity < 0)
    {
        correctedSeparatingVelocity += _restitution * accCausedSepVelocity;
        if (correctedSeparatingVelocity < 0)
        {
            correctedSeparatingVelocity = 0;
        }
    }
    real_t deltaVelocity = correctedSeparatingVelocity - separatingVelocity;

    // impulse = dv / ((m + n) / mn) = dv * mn / (m + n)
    // then each particle gets impulse * inverseMass
    real_t impulse = deltaVelocity / totalInverseMass;
    // Find the amount of impulse per unit of inverse mass
    Vector3 impulsePerIMass = _contactNormal * impulse;

    // Apply the impulse to the particles.
    _particles[0]->AddVelocity(impulsePerIMass * _particles[0]->GetInverseMass());
    if (_particles[1])
    {
        _particles[1]->AddVelocity(impulsePerIMass * -_particles[1]->GetInverseMass());
    }
}

void ParticleContact::_ResolveInterpenetration(real_t delta)
{
    // If there is no penetration, do nothing.
    if (_penetration <= 0)
    {
        return;
    }

    // Two particles with infinite mass can't penetrate each other.
    real_t totalInverseMass = _particles[0]->GetInverseMass();
    if (_particles[1])
    {
        totalInverseMass += _particles[1]->GetInverseMass();
    }
    if (totalInverseMass <= 0)
    {
        return;
    }

    // Find the amount of penetration resolution per unit of inverse mass.
    Vector3 movePerIMass = _contactNormal * (_penetration / totalInverseMass);

    // Calculate the movement amounts.
    _movements[0] = movePerIMass * _particles[0]->GetInverseMass();
    if (_particles[1])
    {
        _movements[1] = movePerIMass * -_particles[1]->GetInverseMass();
    }
    else
    {
        _movements[1].Clear();
    }

    // Apply the penetration resolution.
    _particles[0]->Translate(_movements[0]);
    if (_particles[1])
    {
        _particles[1]->Translate(_movements[1]);
    }
}

ParticleContactResolver::ParticleContactResolver(uint32_t iterations) : _iterations(iterations), _iterationsUsed(0)
{
}

void ParticleContactResolver::Resolve(ParticleContact* contacts, size_t count, real_t delta)
{
    _iterationsUsed = 0;
    while (_iterationsUsed < _iterations)
    {
        // Find the contact with the largest closing velocity.
        real_t max = MAX_REAL;
        size_t maxIndex = count;
        for (size_t i = 0; i < count; i++)
        {
            real_t sepVel = contacts[i]._CalculateSeparatingVelocity();
            if ((sepVel < max) && (sepVel < 0 || contacts[i].GetPenetration() > 0))
            {
                max = sepVel;
                maxIndex = i;
            }
        }
        if (maxIndex == count)
        {
            break;
        }

        // Resolve this contact.
        contacts[maxIndex]._Resolve(delta);

        // Update all other contacts if they share the same particles.
        Vector3* move = contacts[maxIndex]._movements;
        for (size_t i = 0; i < count; i++)
        {
            if (contacts[i].GetFirst() == contacts[maxIndex].GetFirst())
            {
                contacts[i]._penetration -= move[0] * contacts[i].GetContactNormal();
            }
            else if (contacts[i].GetFirst() == contacts[maxIndex].GetSecond())
            {
                contacts[i]._penetration -= move[1] * contacts[i].GetContactNormal();
            }

            if (contacts[i].GetSecond())
            {
                if (contacts[i].GetSecond() == contacts[maxIndex].GetFirst())
                {
                    contacts[i]._penetration += move[0] * contacts[i].GetContactNormal();
                }
                else if (contacts[i].GetSecond() == contacts[maxIndex].GetSecond())
                {
                    contacts[i]._penetration += move[1] * contacts[i].GetContactNormal();
                }
            }
        }

        _iterationsUsed++;
    }
}

void ParticleContactRegistry::Add(ParticleContactGenerator* contactGenerator)
{
    _contactGenerators.push_back(contactGenerator);
}

void ParticleContactRegistry::Remove(ParticleContactGenerator* contactGenerator)
{
    for (auto it = _contactGenerators.begin(); it != _contactGenerators.end(); ++it)
    {
        if (*it == contactGenerator)
        {
            _contactGenerators.erase(it);
            break;
        }
    }
}

void ParticleContactRegistry::Clear()
{
    _contactGenerators.clear();
}

int ParticleContactRegistry::AddContact(ParticleContact* contacts, int limit)
{
    int available = limit;

    for (auto generator : _contactGenerators)
    {
        int added = generator->AddContact(contacts, available);
        available -= added;
        contacts += added;

        if (available <= 0)
        {
            break;
        }
    }

    return limit - available;
}

DPHX_END
