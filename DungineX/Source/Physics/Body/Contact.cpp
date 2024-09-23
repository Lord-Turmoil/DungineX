#include "DgeX/Physics/Body/Contact.h"

DPHX_BEGIN

void Contact::_CalculateInternals(real_t delta)
{
    if (!_bodies[0])
    {
        _SwapBodies();
    }

    DGEX_ASSERT(_bodies[0], "First body of contact is nullptr.");

    _CalculateContactBasis();

    _relativeContactPosition[0] = ContactPoint - _bodies[0]->GetPosition();
    if (_bodies[1])
    {
        _relativeContactPosition[1] = ContactPoint - _bodies[1]->GetPosition();
    }

    _contactVelocity = _CalculateLocalVelocity(0, delta);
    if (_bodies[1])
    {
        _contactVelocity -= _CalculateLocalVelocity(1, delta);
    }

    // Finally, calculate the desired delta velocity to resolve the contact.
    _CalculateDesiredDeltaVelocity(delta);
}

void Contact::_SwapBodies()
{
    ContactNormal.Invert();
    std::swap(_bodies[0], _bodies[1]);
}

void Contact::_MatchAwakeState() const
{
    if (!_bodies[1])
    {
        return;
    }

    bool body0Awake = _bodies[0]->IsAwake();
    bool body1Awake = _bodies[1]->IsAwake();

    if (body0Awake ^ body1Awake)
    {
        if (body0Awake)
        {
            _bodies[1]->IsAwake(true);
        }
        else
        {
            _bodies[0]->IsAwake(true);
        }
    }
}

void Contact::_CalculateDesiredDeltaVelocity(real_t delta)
{
    // A magic number to control the restitution velocity.
    static constexpr real_t sVelocityLimit = 0.25f;

    // Calculate the acceleration induced velocity accumulated this frame.
    real_t velocityFromAcc = 0;
    if (_bodies[0]->IsAwake())
    {
        velocityFromAcc += _bodies[0]->GetLastFrameAcceleration() * delta * ContactNormal;
    }
    if (_bodies[1] && _bodies[1]->IsAwake())
    {
        velocityFromAcc -= _bodies[1]->GetLastFrameAcceleration() * delta * ContactNormal;
    }

    // If the velocity is very slow, limit the restitution.
    real_t correctedRestitution = Restitution;
    if (Math::Abs(_contactVelocity.X) < sVelocityLimit)
    {
        correctedRestitution = static_cast<real_t>(0.0f);
    }

    // Combine the bounce velocity with the removed acceleration velocity.
    _desiredDeltaVelocity = -_contactVelocity.X - correctedRestitution * (_contactVelocity.X - velocityFromAcc);
}

Vector3 Contact::_CalculateLocalVelocity(int bodyIndex, real_t delta) const
{
    RigidBody* body = _bodies[bodyIndex];

    Vector3 velocity = body->GetRotation() % _relativeContactPosition[bodyIndex];
    velocity += body->GetVelocity();

    // Convert the world-space velocity into contact-space velocity.
    Vector3 contactVelocity = _transformMatrix.TransformTranspose(velocity);

    // Get the acceleration caused velocity, which is produced without reaction.
    Vector3 accCausedVelocity = body->GetLastFrameAcceleration() * delta;

    // Velocity in the contact normal direction is ignored as we're only interested
    // in planar acceleration.
    accCausedVelocity.X = 0;

    // Add the planar velocities. If there is enough friction, they will be removed
    // later during velocity resolution.
    contactVelocity += accCausedVelocity;

    return contactVelocity;
}

inline void Contact::_CalculateContactBasis()
{
    Vector3 contactTangent[2];

    // Check whether the Z-axis is nearer to the X or Y axis.
    if (Math::Abs(ContactNormal.X) > Math::Abs(ContactNormal.Y))
    {
        // Scaling factor to ensure the results are normalised.
        const real_t s =
            static_cast<real_t>(1) / Math::Sqrt(ContactNormal.Z * ContactNormal.Z + ContactNormal.X * ContactNormal.X);

        // The new X-axis is at right angles to the world Y-axis.
        contactTangent[0].X = ContactNormal.Z * s;
        contactTangent[0].Y = 0;
        contactTangent[0].Z = -ContactNormal.X * s;

        // The new Y-axis is at right angles to the new X- and Z- axes.
        contactTangent[1].X = ContactNormal.Y * contactTangent[0].X;
        contactTangent[1].Y = ContactNormal.Z * contactTangent[0].X - ContactNormal.X * contactTangent[0].Z;
        contactTangent[1].Z = -ContactNormal.Y * contactTangent[0].X;
    }
    else
    {
        // Scaling factor to ensure the results are normalised.
        const real_t s =
            static_cast<real_t>(1) / Math::Sqrt(ContactNormal.Z * ContactNormal.Z + ContactNormal.Y * ContactNormal.Y);

        // The new X-axis is at right angles to the world X-axis.
        contactTangent[0].X = 0;
        contactTangent[0].Y = -ContactNormal.Z * s;
        contactTangent[0].Z = ContactNormal.Y * s;

        // The new Y-axis is at right angles to the new X- and Z- axes.
        contactTangent[1].X = ContactNormal.Y * contactTangent[0].Z - ContactNormal.Z * contactTangent[0].Y;
        contactTangent[1].Y = -ContactNormal.X * contactTangent[0].Z;
        contactTangent[1].Z = ContactNormal.X * contactTangent[0].Y;
    }

    // Make a matrix from the three vectors.
    _transformMatrix.SetComponents(ContactNormal, contactTangent[0], contactTangent[1]);
}

void Contact::_ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2]) const
{
    Matrix3 inverseInertiaTensor[2];
    inverseInertiaTensor[0] = _bodies[0]->GetInverseInertiaTensorWorld();
    if (_bodies[1])
    {
        inverseInertiaTensor[1] = _bodies[1]->GetInverseInertiaTensorWorld();
    }

    Vector3 contactImpulse;
    if (Friction == static_cast<real_t>(0))
    {
        contactImpulse = _CalculateFrictionlessImpulse(inverseInertiaTensor);
    }
    else
    {
        contactImpulse = _CalculateFrictionImpulse(inverseInertiaTensor);
    }

    // To world-space impulse.
    Vector3 impulse = _transformMatrix.Transform(contactImpulse);

    // Split the impulse into linear and angular.
    Vector3 impulsiveTorque = _relativeContactPosition[0] % impulse;
    rotationChange[0] = inverseInertiaTensor[0].Transform(impulsiveTorque);
    velocityChange[0] = impulse * _bodies[0]->GetInverseMass();

    _bodies[0]->AddVelocity(velocityChange[0]);
    _bodies[0]->AddRotation(rotationChange[0]);

    if (_bodies[1])
    {
        impulsiveTorque = impulse % _relativeContactPosition[1];
        rotationChange[1] = inverseInertiaTensor[1].Transform(impulsiveTorque);
        velocityChange[1] = impulse * -_bodies[1]->GetInverseMass();

        _bodies[1]->AddVelocity(velocityChange[1]);
        _bodies[1]->AddRotation(rotationChange[1]);
    }
}

// TODO: Copied directly from the book, need further understanding.
void Contact::_ApplyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], real_t penetration) const
{
    static constexpr real_t angularLimit = 0.2f;

    real_t angularMove[2];
    real_t linearMove[2];

    real_t totalInertia = 0;
    real_t linearInertia[2];
    real_t angularInertia[2];

    // We need to work out the inertia of each object in the direction
    // of the contact normal, due to angular inertia only.
    for (unsigned i = 0; i < 2; i++)
    {
        if (_bodies[i])
        {
            Matrix3 inverseInertiaTensor = _bodies[i]->GetInverseInertiaTensorWorld();

            // Use the same procedure as for calculating frictionless
            // velocity change to work out the angular inertia.
            Vector3 angularInertiaWorld = _relativeContactPosition[i] % ContactNormal;
            angularInertiaWorld = inverseInertiaTensor.Transform(angularInertiaWorld);
            angularInertiaWorld = angularInertiaWorld % _relativeContactPosition[i];
            angularInertia[i] = angularInertiaWorld * ContactNormal;

            // The linear component is simply the inverse mass
            linearInertia[i] = _bodies[i]->GetInverseMass();

            // Keep track of the total inertia from all components
            totalInertia += linearInertia[i] + angularInertia[i];

            // We break the loop here so that the totalInertia value is
            // completely calculated (by both iterations) before continuing.
        }
    }

    // Loop through again calculating and applying the changes
    for (unsigned i = 0; i < 2; i++)
    {
        if (_bodies[i])
        {
            // The linear and angular movements required are in proportion to
            // the two inverse inertias.
            real_t sign = (i == 0) ? 1 : -1;
            angularMove[i] = sign * penetration * (angularInertia[i] / totalInertia);
            linearMove[i] = sign * penetration * (linearInertia[i] / totalInertia);

            // To avoid angular projections that are too great (when mass is large
            // but inertia tensor is small) limit the angular move.
            Vector3 projection = _relativeContactPosition[i];
            projection.AddScaledVector(ContactNormal, -_relativeContactPosition[i].ScalarProduct(ContactNormal));

            // Use the small angle approximation for the sine of the angle (i.e.
            // the Magnitude would be sine(angularLimit) * projection.Magnitude
            // but we approximate sine(angularLimit) to angularLimit).
            real_t maxMagnitude = angularLimit * projection.Magnitude();

            if (angularMove[i] < -maxMagnitude)
            {
                real_t totalMove = angularMove[i] + linearMove[i];
                angularMove[i] = -maxMagnitude;
                linearMove[i] = totalMove - angularMove[i];
            }
            else if (angularMove[i] > maxMagnitude)
            {
                real_t totalMove = angularMove[i] + linearMove[i];
                angularMove[i] = maxMagnitude;
                linearMove[i] = totalMove - angularMove[i];
            }

            // We have the linear amount of movement required by turning
            // the rigid _bodies (in angularMove[i]). We now need to
            // calculate the desired rotation to achieve that.
            if (angularMove[i] == 0)
            {
                // Easy case - no angular movement means no rotation.
                angularChange[i].Clear();
            }
            else
            {
                // Work out the direction we'd like to rotate in.
                Vector3 targetAngularDirection = _relativeContactPosition[i].VectorProduct(ContactNormal);

                Matrix3 inverseInertiaTensor = _bodies[i]->GetInverseInertiaTensorWorld();

                // Work out the direction we'd need to rotate to achieve that
                angularChange[i] =
                    inverseInertiaTensor.Transform(targetAngularDirection) * (angularMove[i] / angularInertia[i]);
            }

            // Velocity change is easier - it is just the linear movement
            // along the contact normal.
            linearChange[i] = ContactNormal * linearMove[i];

            // Now we can start to apply the values we've calculated.
            // Apply the linear movement
            Vector3 pos = _bodies[i]->GetPosition();
            pos.AddScaledVector(ContactNormal, linearMove[i]);
            _bodies[i]->SetPosition(pos);

            // And the change in orientation
            Quaternion q = _bodies[i]->GetOrientation();
            q.AddScaledVector(angularChange[i], ((real_t)1.0f));
            _bodies[i]->SetOrientation(q);

            // We need to calculate the derived data for any _bodies that is
            // asleep, so that the changes are reflected in the object's
            // data. Otherwise the resolution will not change the position
            // of the object, and the next collision detection round will
            // have the same penetration.
            if (!_bodies[i]->IsAwake())
            {
                _bodies[i]->CalculateDerivedData();
            }
        }
    }
}

inline Vector3 Contact::_CalculateFrictionlessImpulse(Matrix3 inverseInertiaTensor[2]) const
{
    Vector3 contactImpulse;

    // Change in velocity in world space for unit impulse in the direction of
    // the contact normal.
    Vector3 deltaVelocityWorld = _relativeContactPosition[0] % ContactNormal;
    deltaVelocityWorld = inverseInertiaTensor[0].Transform(deltaVelocityWorld);
    deltaVelocityWorld = deltaVelocityWorld % _relativeContactPosition[0];

    real_t deltaVelocity = deltaVelocityWorld * ContactNormal;

    // Add the linear component of velocity change.
    deltaVelocity += _bodies[0]->GetInverseMass();

    if (_bodies[1])
    {
        deltaVelocityWorld = _relativeContactPosition[1] % ContactNormal;
        deltaVelocityWorld = inverseInertiaTensor[1].Transform(deltaVelocityWorld);
        deltaVelocityWorld = deltaVelocityWorld % _relativeContactPosition[1];

        deltaVelocity += deltaVelocityWorld * ContactNormal;

        deltaVelocity += _bodies[1]->GetInverseMass();
    }

    contactImpulse.X = _desiredDeltaVelocity / deltaVelocity;
    contactImpulse.Y = 0;
    contactImpulse.Z = 0;

    return contactImpulse;
}

inline Vector3 Contact::_CalculateFrictionImpulse(Matrix3 inverseInertiaTensor[2]) const
{
    real_t inverseMass = _bodies[0]->GetInverseMass();

    // The equivalent of a cross product in matrices is multiplication
    // by a skew symmetric matrix - we build the matrix for converting
    // between linear and angular quantities.
    Matrix3 impulseToTorque;
    impulseToTorque.SetSkewSymmetric(_relativeContactPosition[0]);

    // Build the matrix to convert contact impulse to change in velocity
    // in world coordinates.
    Matrix3 deltaVelocityWorld = impulseToTorque;
    deltaVelocityWorld *= inverseInertiaTensor[0];
    deltaVelocityWorld *= impulseToTorque;
    deltaVelocityWorld *= -1;

    // Check if we need to add body two's data
    if (_bodies[1])
    {
        // Set the cross product matrix
        impulseToTorque.SetSkewSymmetric(_relativeContactPosition[1]);

        // Calculate the velocity change matrix
        Matrix3 deltaVelWorld2 = impulseToTorque;
        deltaVelWorld2 *= inverseInertiaTensor[1];
        deltaVelWorld2 *= impulseToTorque;
        deltaVelWorld2 *= -1;

        // Add to the total delta velocity.
        deltaVelocityWorld += deltaVelWorld2;

        // Add to the inverse mass
        inverseMass += _bodies[1]->GetInverseMass();
    }

    // Do a change of basis to convert into contact coordinates.
    Matrix3 deltaVelocity = _transformMatrix.Transpose();
    deltaVelocity *= deltaVelocityWorld;
    deltaVelocity *= _transformMatrix;

    // Add in the linear velocity change
    deltaVelocity.Data[0] += inverseMass;
    deltaVelocity.Data[4] += inverseMass;
    deltaVelocity.Data[8] += inverseMass;

    // Invert to get the impulse needed per unit velocity
    Matrix3 impulseMatrix = deltaVelocity.Inverse();

    // Find the target velocities to kill
    Vector3 velKill(_desiredDeltaVelocity, -_contactVelocity.Y, -_contactVelocity.Z);

    // Find the impulse to kill target velocities
    Vector3 contactImpulse = impulseMatrix.Transform(velKill);

    // Check for exceeding friction
    real_t planarImpulse = Math::Sqrt(contactImpulse.Y * contactImpulse.Y + contactImpulse.Z * contactImpulse.Z);
    if (planarImpulse > contactImpulse.X * Friction)
    {
        // We need to use dynamic friction
        contactImpulse.Y /= planarImpulse;
        contactImpulse.Z /= planarImpulse;

        contactImpulse.X = deltaVelocity.Data[0] + deltaVelocity.Data[1] * Friction * contactImpulse.Y +
                           deltaVelocity.Data[2] * Friction * contactImpulse.Z;
        contactImpulse.X = _desiredDeltaVelocity / contactImpulse.X;
        contactImpulse.Y *= Friction * contactImpulse.X;
        contactImpulse.Z *= Friction * contactImpulse.X;
    }

    return contactImpulse;
}

DPHX_END
