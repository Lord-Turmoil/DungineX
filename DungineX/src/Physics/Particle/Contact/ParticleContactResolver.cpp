#include "DgeX/Physics/Particle/Contact/ParticleContactResolver.h"
#include "DgeX/Physics/Particle/Contact/ParticleContact.h"

DPHX_BEGIN

ParticleContactResolver::ParticleContactResolver(uint32_t iterations) : _iterations(iterations)
{
}

void ParticleContactResolver::Resolve(ParticleContact* contacts, uint32_t count, real_t delta) const
{
    if (count == 0)
    {
        return;
    }

    uint32_t iterationsUsed = 0;
    while (iterationsUsed < _iterations)
    {
        // Find the contact with the largest closing velocity.
        real_t max = MAX_REAL;
        uint32_t maxIndex = count;
        for (uint32_t i = 0; i < count; i++)
        {
            real_t sepVel = contacts[i]._CalculateSeparatingVelocity();
            if ((sepVel < max) && (sepVel < 0 || contacts[i].Penetration > 0))
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
        for (uint32_t i = 0; i < count; i++)
        {
            if (contacts[i].GetFirst() == contacts[maxIndex].GetFirst())
            {
                contacts[i].Penetration -= move[0] * contacts[i].ContactNormal;
            }
            else if (contacts[i].GetFirst() == contacts[maxIndex].GetSecond())
            {
                contacts[i].Penetration -= move[1] * contacts[i].ContactNormal;
            }

            if (contacts[i].GetSecond())
            {
                if (contacts[i].GetSecond() == contacts[maxIndex].GetFirst())
                {
                    contacts[i].Penetration += move[0] * contacts[i].ContactNormal;
                }
                else if (contacts[i].GetSecond() == contacts[maxIndex].GetSecond())
                {
                    contacts[i].Penetration += move[1] * contacts[i].ContactNormal;
                }
            }
        }

        iterationsUsed++;
    }
}

DPHX_END
