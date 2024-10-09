#include "DgeX/Physics/Particle/Contact/ParticleContactGenerator.h"
#include "DgeX/Physics/Particle/Contact/ParticleContact.h"

DPHX_BEGIN

void ParticleContactRegistry::Add(ParticleContactGenerator* contactGenerator)
{
    _contactGenerators.PushBack(*contactGenerator);
}

void ParticleContactRegistry::Remove(ParticleContactGenerator* contactGenerator)
{
    _contactGenerators.Erase(contactGenerator);
}

void ParticleContactRegistry::Clear()
{
    _contactGenerators.Clear();
}

uint32_t ParticleContactRegistry::AddContact(ParticleContact* contacts, uint32_t limit) const
{
    uint32_t available = limit;

    for (auto& generator : _contactGenerators)
    {
        uint32_t added = generator.AddContact(contacts, available);
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
