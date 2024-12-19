#include "DgeX/Physics/Particle/Contact/ParticleLink.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

real_t ParticleAnchoredLink::_Length() const
{
    return (_particle->GetPosition() - *_anchor).Magnitude();
}

real_t ParticleLink::_Length() const
{
    return (_particles[0]->GetPosition() - _particles[1]->GetPosition()).Magnitude();
}

DPHX_END
