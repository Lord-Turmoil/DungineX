#include "DgeX/Physics/Body/RigidBody.h"

DPHX_BEGIN

static void CalculateTransformMatrix(Matrix4& transformMatrix, const Vector3& position, const Quaternion& orientation);
static void TransformInertiaTensor(Matrix3& inverseInertiaTensorWorld, const Quaternion& orientation,
                                   const Matrix3& inverseInertiaTensor, const Matrix4& transformMatrix);

static real_t sSleepEpsilon = static_cast<real_t>(0.3);

void RigidBody::Integrate(real_t delta)
{
    if (!_isAwake)
    {
        return;
    }

    // Integrate linear velocity.
    _lastFrameAcceleration = _acceleration;
    _lastFrameAcceleration.AddScaledVector(_accumulatedForce, _inverseMass);
    _velocity.AddScaledVector(_lastFrameAcceleration, delta);
    _velocity *= Math::Pow(_linearDamping, delta);

    // Integrate angular velocity.
    Vector3 angularAcceleration = _inverseInertiaTensorWorld.Transform(_accumulatedTorque);
    _rotation.AddScaledVector(angularAcceleration, delta);
    _rotation *= Math::Pow(_angularDamping, delta);

    // Update the position.
    _position.AddScaledVector(_velocity, delta);

    // Update angular position.
    _orientation.AddScaledVector(_rotation, delta);

    // Normalise the orientation, and update the matrices with the new
    // position and orientation
    CalculateDerivedData();

    // Clear accumulators.
    ClearAccumulators();

    // Update the kinetic energy store, and possibly put the body to
    // sleep.
    if (_canSleep)
    {
        real_t currentMotion = _velocity.ScalarProduct(_velocity) + _rotation.ScalarProduct(_rotation);
        real_t bias = Math::Pow(0.5, delta);
        _motion = bias * _motion + (1 - bias) * currentMotion;

        if (_motion < sSleepEpsilon)
        {
            _isAwake = false;
        }
        else if (_motion > 10 * sSleepEpsilon)
        {
            _motion = 10 * sSleepEpsilon;
        }
    }
}

void RigidBody::CalculateDerivedData()
{
    _orientation.Normalize();

    CalculateTransformMatrix(_transformMatrix, _position, _orientation);
    TransformInertiaTensor(_inverseInertiaTensorWorld, _orientation, _inverseInertiaTensor, _transformMatrix);
}

void RigidBody::ApplyForceAtWorldPoint(const Vector3& force, const Vector3& point)
{
    _accumulatedForce += force;
    _accumulatedTorque += (point - _position) % force; // cross product
    _isAwake = true;
}

void RigidBody::ApplyForceAtLocalPoint(const Vector3& force, const Vector3& point)
{
    ApplyForceAtWorldPoint(force, ToWorldPoint(point));
}

void CalculateTransformMatrix(Matrix4& transformMatrix, const Vector3& position, const Quaternion& orientation)
{
    transformMatrix.Data[0] = 1 - 2 * orientation.J * orientation.J - 2 * orientation.K * orientation.K;
    transformMatrix.Data[1] = 2 * orientation.I * orientation.J - 2 * orientation.R * orientation.K;
    transformMatrix.Data[2] = 2 * orientation.I * orientation.K + 2 * orientation.R * orientation.J;
    transformMatrix.Data[3] = position.X;

    transformMatrix.Data[4] = 2 * orientation.I * orientation.J + 2 * orientation.R * orientation.K;
    transformMatrix.Data[5] = 1 - 2 * orientation.I * orientation.I - 2 * orientation.K * orientation.K;
    transformMatrix.Data[6] = 2 * orientation.J * orientation.K - 2 * orientation.R * orientation.I;
    transformMatrix.Data[7] = position.Y;

    transformMatrix.Data[8] = 2 * orientation.I * orientation.K - 2 * orientation.R * orientation.J;
    transformMatrix.Data[9] = 2 * orientation.J * orientation.K + 2 * orientation.R * orientation.I;
    transformMatrix.Data[10] = 1 - 2 * orientation.I * orientation.I - 2 * orientation.J * orientation.J;
    transformMatrix.Data[11] = position.Z;
}

void TransformInertiaTensor(Matrix3& inverseInertiaTensorWorld, const Quaternion& orientation,
                            const Matrix3& inverseInertiaTensor, const Matrix4& transformMatrix)
{
    real_t t4 = transformMatrix.Data[0] * inverseInertiaTensor.Data[0] +
                transformMatrix.Data[1] * inverseInertiaTensor.Data[3] +
                transformMatrix.Data[2] * inverseInertiaTensor.Data[6];
    real_t t9 = transformMatrix.Data[0] * inverseInertiaTensor.Data[1] +
                transformMatrix.Data[1] * inverseInertiaTensor.Data[4] +
                transformMatrix.Data[2] * inverseInertiaTensor.Data[7];
    real_t t14 = transformMatrix.Data[0] * inverseInertiaTensor.Data[2] +
                 transformMatrix.Data[1] * inverseInertiaTensor.Data[5] +
                 transformMatrix.Data[2] * inverseInertiaTensor.Data[8];
    real_t t28 = transformMatrix.Data[4] * inverseInertiaTensor.Data[0] +
                 transformMatrix.Data[5] * inverseInertiaTensor.Data[3] +
                 transformMatrix.Data[6] * inverseInertiaTensor.Data[6];
    real_t t33 = transformMatrix.Data[4] * inverseInertiaTensor.Data[1] +
                 transformMatrix.Data[5] * inverseInertiaTensor.Data[4] +
                 transformMatrix.Data[6] * inverseInertiaTensor.Data[7];
    real_t t38 = transformMatrix.Data[4] * inverseInertiaTensor.Data[2] +
                 transformMatrix.Data[5] * inverseInertiaTensor.Data[5] +
                 transformMatrix.Data[6] * inverseInertiaTensor.Data[8];
    real_t t52 = transformMatrix.Data[8] * inverseInertiaTensor.Data[0] +
                 transformMatrix.Data[9] * inverseInertiaTensor.Data[3] +
                 transformMatrix.Data[10] * inverseInertiaTensor.Data[6];
    real_t t57 = transformMatrix.Data[8] * inverseInertiaTensor.Data[1] +
                 transformMatrix.Data[9] * inverseInertiaTensor.Data[4] +
                 transformMatrix.Data[10] * inverseInertiaTensor.Data[7];
    real_t t62 = transformMatrix.Data[8] * inverseInertiaTensor.Data[2] +
                 transformMatrix.Data[9] * inverseInertiaTensor.Data[5] +
                 transformMatrix.Data[10] * inverseInertiaTensor.Data[8];

    inverseInertiaTensorWorld.Data[0] =
        t4 * transformMatrix.Data[0] + t9 * transformMatrix.Data[1] + t14 * transformMatrix.Data[2];
    inverseInertiaTensorWorld.Data[1] =
        t4 * transformMatrix.Data[4] + t9 * transformMatrix.Data[5] + t14 * transformMatrix.Data[6];
    inverseInertiaTensorWorld.Data[2] =
        t4 * transformMatrix.Data[8] + t9 * transformMatrix.Data[9] + t14 * transformMatrix.Data[10];
    inverseInertiaTensorWorld.Data[3] =
        t28 * transformMatrix.Data[0] + t33 * transformMatrix.Data[1] + t38 * transformMatrix.Data[2];
    inverseInertiaTensorWorld.Data[4] =
        t28 * transformMatrix.Data[4] + t33 * transformMatrix.Data[5] + t38 * transformMatrix.Data[6];
    inverseInertiaTensorWorld.Data[5] =
        t28 * transformMatrix.Data[8] + t33 * transformMatrix.Data[9] + t38 * transformMatrix.Data[10];
    inverseInertiaTensorWorld.Data[6] =
        t52 * transformMatrix.Data[0] + t57 * transformMatrix.Data[1] + t62 * transformMatrix.Data[2];
    inverseInertiaTensorWorld.Data[7] =
        t52 * transformMatrix.Data[4] + t57 * transformMatrix.Data[5] + t62 * transformMatrix.Data[6];
    inverseInertiaTensorWorld.Data[8] =
        t52 * transformMatrix.Data[8] + t57 * transformMatrix.Data[9] + t62 * transformMatrix.Data[10];
}

void SetSleepEpsilon(real_t epsilon)
{
    sSleepEpsilon = epsilon;
}

real_t GetSleepEpsilon()
{
    return sSleepEpsilon;
}

DPHX_END
