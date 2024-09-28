#pragma once

#include "DgeX/Physics/phxpch.h"

DPHX_BEGIN

class RigidBody;

class Contact
{
    friend class ContactResolver;

public:
    // clang-format off
    void SetFirst(RigidBody* body) { _bodies[0] = body; }
    RigidBody* GetFirst() const { return _bodies[0]; }

    void SetSecond(RigidBody* body) { _bodies[1] = body; }
    RigidBody* GetSecond() const { return _bodies[1]; }

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

/**
 * @brief Responsible for resolving contacts between rigid bodies.
 *
 * @paragraph algorithm
 *
 * This resolver solves contact locally, which means solving one contact can
 * make other contacts worse. To solve this, the resolver will iterate through
 * all contacts multiple times to best approximate the global solution.
 *
 * @paragraph limitation
 *
 * This algorithm does not perform well for inter-related contacts. For example,
 * stacked boxes may jiggle a little, resulting in collapse.
 *
 * Also, resolving contacts may cause object translate regardless of friction.
 */
class ContactResolver
{
public:
    ContactResolver(uint32_t iterations);
    virtual ~ContactResolver() = default;

    // clang-format off
    void SetVelocityIterations(uint32_t iterations) { _velocityIterations = iterations; }
    void SetPositionIterations(uint32_t iterations) { _positionIterations = iterations; }

    // epsilon should be greater than 0
    void SetVelocityEpsilon(real_t epsilon) { _velocityEpsilon = epsilon; }
    // epsilon should be greater than 0
    void SetPositionEpsilon(real_t epsilon) { _positionEpsilon = epsilon; }

    // clang-format on

    void Resolve(Contact* contacts, uint32_t count, real_t delta);

protected:
    /**
     * @brief
     * Prepare contacts' data and set their awake state.
     */
    void _PrepareContacts(Contact* contacts, uint32_t count, real_t delta);

    /**
     * @brief Resolve velocity constraints.
     */
    void _AdjustVelocities(Contact* contacts, uint32_t count, real_t delta) const;

    /**
     * @brief Resolve position constraints.
     */
    void _AdjustPositions(Contact* contacts, uint32_t count, real_t delta) const;

protected:
    /**
     * @brief Iteration 0 will use the 2 * count as the number of iterations.
     */
    uint32_t _velocityIterations = 0;
    uint32_t _positionIterations = 0;

    /**
     * @brief
     * Taking velocity smaller than this value as zero to avoid instability.
     */
    real_t _velocityEpsilon = 0.01;

    /**
     * @brief
     * Taking position smaller than this value as zero to avoid instability.
     */
    real_t _positionEpsilon = 0.01;
};

/**
 * @brief
 * Base class for contact generators that generate contacts between rigid bodies.
 */
class ContactGenerator
{
public:
    virtual ~ContactGenerator() = default;
    /**
     * @brief
     * Add contacts to the contact list with the given limit.
     * The contact should be a pointer in an array of contacts.
     *
     * @note
     * The limit is the maximum number of contacts that can be written, it
     * is greater than or equal to 1.
     */
    virtual uint32_t AddContact(Contact* contact, uint32_t limit) const = 0;
};

DPHX_END
