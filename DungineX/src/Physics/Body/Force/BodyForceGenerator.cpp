#include "DgeX/Physics/Body/Force/BodyForceGenerator.h"

#include "DgeX/Physics/Body/RigidBody.h"

DPHX_BEGIN

void ForceRegistry::Add(RigidBody* particle, BodyForceGenerator* forceGenerator)
{
    _registrations.emplace_back(particle, forceGenerator);
}

void ForceRegistry::Remove(RigidBody* particle, BodyForceGenerator* forceGenerator)
{
    for (auto it = _registrations.begin(); it != _registrations.end(); ++it)
    {
        if (it->RigidBody == particle && it->ForceGenerator == forceGenerator)
        {
            _registrations.erase(it);
            break;
        }
    }
}

void ForceRegistry::Clear()
{
    _registrations.clear();
}

void ForceRegistry::UpdateForce(real_t delta) const
{
    for (auto registration : _registrations)
    {
        registration.UpdateForce(delta);
    }
}

DPHX_END
