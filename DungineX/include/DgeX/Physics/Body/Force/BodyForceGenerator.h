#pragma once

#include <vector>

#include "DgeX/Physics/Core/Precision.h"

DPHX_BEGIN

class RigidBody;

/**
 * @brief Force generator for rigid body.
 */
class BodyForceGenerator
{
public:
    virtual ~BodyForceGenerator() = default;

    virtual void UpdateForce(RigidBody* body, real_t delta) = 0;
};

/**
 * @brief Force registry for rigid body.
 */
class ForceRegistry
{
public:
    void Add(RigidBody* particle, BodyForceGenerator* forceGenerator);
    void Remove(RigidBody* particle, BodyForceGenerator* forceGenerator);
    void Clear();

    void UpdateForce(real_t delta) const;

protected:
    struct ForceRegistration
    {
        RigidBody* RigidBody;
        BodyForceGenerator* ForceGenerator;

        ForceRegistration(DPHX RigidBody* body, BodyForceGenerator* forceGenerator)
            : RigidBody(body), ForceGenerator(forceGenerator)
        {
        }

        void UpdateForce(real_t delta) const
        {
            ForceGenerator->UpdateForce(RigidBody, delta);
        }
    };

    std::vector<ForceRegistration> _registrations;
};

DPHX_END
