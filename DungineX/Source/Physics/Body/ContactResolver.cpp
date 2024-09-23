#include "DgeX/Physics/Body/Contact.h"

DPHX_BEGIN

ContactResolver::ContactResolver(uint32_t iterations)
{
    SetVelocityIterations(iterations);
    SetPositionIterations(iterations);
}

void ContactResolver::Resolve(Contact* contacts, uint32_t count, real_t delta)
{
    if (count == 0)
    {
        return;
    }

    _PrepareContacts(contacts, count, delta);
    _AdjustPositions(contacts, count, delta);
    _AdjustVelocities(contacts, count, delta);
}

void ContactResolver::_PrepareContacts(Contact* contacts, uint32_t count, real_t delta)
{
    auto end = contacts + count;
    for (Contact* contact = contacts; contact != end; contact++)
    {
        contact->_CalculateInternals(delta);
    }
}

void ContactResolver::_AdjustVelocities(Contact* contacts, uint32_t count, real_t delta) const
{
    Vector3 velocityChange[2];
    Vector3 rotationChange[2];

    uint32_t velocityIterationsUsed = 0;
    while (velocityIterationsUsed < _velocityIterations)
    {
        real_t max = _velocityEpsilon;
        uint32_t maxIndex = count;

        for (uint32_t i = 0; i < count; i++)
        {
            if (contacts[i]._desiredDeltaVelocity > max)
            {
                max = contacts[i]._desiredDeltaVelocity;
                maxIndex = i;
            }
        }
        if (maxIndex == count)
        {
            break;
        }

        contacts[maxIndex]._MatchAwakeState();

        contacts[maxIndex]._ApplyVelocityChange(velocityChange, rotationChange);

        for (uint32_t i = 0; i < count; i++)
        {
            for (unsigned j = 0; j < 2; j++)
            {
                if (!contacts[i]._bodies[j])
                {
                    continue;
                }

                for (unsigned k = 0; k < 2; k++)
                {
                    if (contacts[i]._bodies[j] == contacts[maxIndex]._bodies[k])
                    {
                        Vector3 deltaVelocity = velocityChange[k] + rotationChange[k].VectorProduct(
                                                                        contacts[i]._relativeContactPosition[j]);
                        contacts[i]._contactVelocity +=
                            contacts[i]._transformMatrix.TransformTranspose(deltaVelocity) * (j ? -1 : 1);
                        contacts[i]._CalculateDesiredDeltaVelocity(delta);
                    }
                }
            }
        }

        velocityIterationsUsed++;
    }
}

void ContactResolver::_AdjustPositions(Contact* contacts, uint32_t count, real_t delta) const
{
    Vector3 linearChange[2];
    Vector3 angularChange[2];

    // iteratively resolve inter-penetrations in order of severity.
    uint32_t positionIterationsUsed = 0;
    while (positionIterationsUsed < _positionIterations)
    {
        // Find the biggest Penetration
        real_t max = _positionEpsilon;
        uint32_t maxIndex = count;
        for (uint32_t i = 0; i < count; i++)
        {
            if (contacts[i].Penetration > max)
            {
                max = contacts[i].Penetration;
                maxIndex = i;
            }
        }
        if (maxIndex == count)
        {
            break;
        }

        // Match the awake state at the contact
        contacts[maxIndex]._MatchAwakeState();

        // Resolve the Penetration.
        contacts[maxIndex]._ApplyPositionChange(linearChange, angularChange, max);

        // Again this action may have changed the Penetration of other
        // bodies, so we update contacts.
        for (uint32_t i = 0; i < count; i++)
        {
            // Check each _bodies in the contact
            for (unsigned j = 0; j < 2; j++)
            {
                if (!contacts[i]._bodies[j])
                {
                    continue;
                }

                // Check for a match with each _bodies in the newly
                // resolved contact
                for (unsigned k = 0; k < 2; k++)
                {
                    if (contacts[i]._bodies[j] == contacts[maxIndex]._bodies[k])
                    {
                        Vector3 deltaPosition =
                            linearChange[k] + angularChange[k].VectorProduct(contacts[i]._relativeContactPosition[j]);

                        // The sign of the change is positive if we're
                        // dealing with the second _bodies in a contact
                        // and negative otherwise (because we're
                        // subtracting the resolution).
                        contacts[i].Penetration +=
                            deltaPosition.ScalarProduct(contacts[i].ContactNormal) * (j ? 1 : -1);
                    }
                }
            }
        }

        positionIterationsUsed++;
    }
}

DPHX_END
