#include "DgeX/Physics/Particle/Contact/ParticlePanel.h"
#include "DgeX/Physics/Particle/Contact/ParticleContact.h"
#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

uint32_t ParticlePanel::AddContact(ParticleContact* contact, uint32_t limit) const
{
    Vector3 position = Particle->GetPosition() - Anchor;
    real_t penetration = -(position * Normal);

    if (penetration <= 0)
    {
        return 0;
    }

    contact->SetFirst(Particle);
    contact->SetSecond(nullptr);
    contact->ContactNormal = Normal;
    contact->Penetration = penetration;
    contact->Restitution = Restitution;

    return 1;
}

uint32_t ParticlesPanel::AddContact(ParticleContact* contact, uint32_t limit) const
{
    uint32_t added = 0;

    Particle* end = _particles + _count;
    for (Particle* particle = _particles; particle != end && added <= limit; particle++)
    {
        Vector3 position = particle->GetPosition() - Anchor;
        real_t penetration = -(position * Normal);
        if (penetration <= 0)
        {
            continue;
        }

        contact->SetFirst(particle);
        contact->SetSecond(nullptr);
        contact->ContactNormal = Normal;
        contact->Penetration = penetration;
        contact->Restitution = Restitution;

        contact++;
        added++;
    }

    return added;
}

DPHX_END
