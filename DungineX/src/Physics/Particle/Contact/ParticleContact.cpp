#include <algorithm>

#include "DgeX/Physics/Particle/Contact/ParticleContact.h"
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
    return relativeVelocity * ContactNormal;
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
    real_t correctedSeparatingVelocity = -separatingVelocity * Restitution;

    // Acceleration may cause static objects to have a closing velocity which
    // may cause the particles to penetrate each other.
    Vector3 accCausedVelocity = _particles[0]->GetAcceleration();
    if (_particles[1])
    {
        accCausedVelocity -= _particles[1]->GetAcceleration();
    }

    // Avoid the velocity due to acceleration so that the particles can be more 'static'.
    real_t accCausedSepVelocity = accCausedVelocity * ContactNormal * delta;
    if (accCausedSepVelocity < 0)
    {
        correctedSeparatingVelocity += Restitution * accCausedSepVelocity;
        correctedSeparatingVelocity = std::max<real_t>(correctedSeparatingVelocity, 0);
    }
    real_t deltaVelocity = correctedSeparatingVelocity - separatingVelocity;

    // impulse = dv / ((m + n) / mn) = dv * mn / (m + n)
    // then each particle gets impulse * inverseMass
    real_t impulse = deltaVelocity / totalInverseMass;
    // Find the amount of impulse per unit of inverse mass
    Vector3 impulsePerIMass = ContactNormal * impulse;

    // Apply the impulse to the particles.
    _particles[0]->AddVelocity(impulsePerIMass * _particles[0]->GetInverseMass());
    if (_particles[1])
    {
        _particles[1]->AddVelocity(impulsePerIMass * -_particles[1]->GetInverseMass());
    }
}

void ParticleContact::_ResolveInterpenetration(real_t /*delta*/)
{
    // If there is no penetration, do nothing.
    if (Penetration <= 0)
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
    Vector3 movePerIMass = ContactNormal * (Penetration / totalInverseMass);

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

DPHX_END
