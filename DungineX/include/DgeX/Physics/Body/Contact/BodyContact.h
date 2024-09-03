#pragma once

#include "DgeX/Physics/Core/Precision.h"
#include "DgeX/Physics/Core/Types/Matrix.h"
#include "DgeX/Physics/Core/Types/Vector.h"

DPHX_BEGIN

class RigidBody;

class BodyContact
{
    friend class BodyContactResolver;

public:
    // clang-format off
    void First(RigidBody* body) { _bodies[0] = body; }
    RigidBody* First() const { return _bodies[0]; }

    void Second(RigidBody* body) { _bodies[1] = body; }
    RigidBody* Second() const { return _bodies[1]; }

    // clang-format on

    real_t Friction;
    real_t Restitution;

    /**
     * @brief Contact point in world coordinates.
     */
    Vector3 ContactPoint;

    /**
     * @brief Direction of the contact in world coordinates.
     */
    Vector3 ContactNormal;

    /**
     * @brief Penetration at the contact point.
     */
    real_t Penetration;

protected:
    /**
     * @brief Calculate internal transient data from state data.
     */
    void _CalculateInternals(real_t delta);

    /**
     * @brief Swap two bodies in this contact.
     * @note The contact normal will also be reversed.
     * @warning The internal data should be manually calculate afterward.
     */
    void _SwapBodies();

    /**
     * @brief
     * Two bodies involved in the contact should both be awake if any of them
     * is awake. However, if it is contact with the world, then the body can
     * remain asleep.
     */
    void _MatchAwakeState() const;

    void _CalculateDesiredDeltaVelocity(real_t delta);

    Vector3 _CalculateLocalVelocity(int bodyIndex, real_t delta) const;

    /**
     * @brief
     * Calculates an orthonormal basis for the contact point, based on the
     * primary friction direction (for anisotropic friction) or a random
     * orientation (for isotropic friction).
     */
    void _CalculateContactBasis();

    /**
     * @brief
     * Performs an inertia-weighted impulse based resolution of this contact alone.
     */
    void _ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]) const;

    /**
     * @brief
     * Performs an inertia-weighted penetration resolution of this contact alone.
     */
    void _ApplyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], real_t penetration) const;

    /**
     * @brief
     * Calculates the impulse needed to resolve this contact without consideration
     * of friction.
     */
    Vector3 _CalculateFrictionlessImpulse(Matrix3 inverseInertiaTensor[2]) const;

    /**
     * @brief
     * Calculates the impulse needed to resolve this contact with friction.
     */
    Vector3 _CalculateFrictionImpulse(Matrix3 inverseInertiaTensor[2]) const;

protected:
    RigidBody* _bodies[2];

    /**
     * @brief Transform local contact point to world space.
     */
    Matrix3 _transformMatrix;

    /**
     * @brief Closing velocity at contact point.
     */
    Vector3 _contactVelocity;

    /**
     * @brief
     * Holds the required change in velocity for this contact to be resolved.
     */
    real_t _desiredDeltaVelocity;

    /**
     * @brief
     * The relative position of the contact point to each body in world
     * space.
     */
    Vector3 _relativeContactPosition[2];
};

DPHX_END
