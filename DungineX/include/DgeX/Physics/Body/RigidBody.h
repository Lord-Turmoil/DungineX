#pragma once

#include "DgeX/Physics/Core/Base.h"
#include "DgeX/Physics/Core/Types/Matrix.h"

DPHX_BEGIN

// clang-format off
/**
 * @brief
 * RigidBody represent an object with mass and collision shape.
 */
class RigidBody : public MassObject
{
public:
    ~RigidBody() override = default;

    void Integrate(real_t delta) override;

    void CalculateDerivedData();

    void SetInertiaTensor(const Matrix3& inertiaTensor) { _inverseInertiaTensor = inertiaTensor.Inverse(); }
    Matrix3 GetInertiaTensor() const { return _inverseInertiaTensor.Inverse(); }
    Matrix3 GetInertiaTensorWorld() const { return _inverseInertiaTensorWorld.Inverse(); }

    void SetInverseInertiaTensor(const Matrix3& inverseInertiaTensor) { _inverseInertiaTensor = inverseInertiaTensor; }
    const Matrix3& GetInverseInertiaTensor() const { return _inverseInertiaTensor; }
    const Matrix3& GetInverseInertiaTensorWorld() const { return _inverseInertiaTensorWorld; }

    void SetDamping(real_t linear, real_t angular) { SetLinearDamping(linear); SetAngularDamping(angular); }
    void SetLinearDamping(real_t linear) { _linearDamping = linear; }
    real_t GetLinearDamping() const { return _linearDamping; }
    void SetAngularDamping(real_t angular) { _angularDamping = angular; }
    real_t GetAngularDamping() const { return _angularDamping; }

    void SetOrientation(const Quaternion& orientation) { _orientation = orientation; }
    void SetOrientation(real_t r, real_t i, real_t j, real_t k) { _orientation.Set(r, i, j, k); }
    const Quaternion& GetOrientation() const { return _orientation; }

    const Matrix4& GetTransformMatrix() const { return _transformMatrix; }

    /**
     * @brief Set the rotation in world space.
     */
    void SetRotation(const Vector3& rotation) { _rotation = rotation; }
    void SetRotation(real_t x, real_t y, real_t z) { _rotation.Set(x, y, z); }
    void AddRotation(const Vector3& rotation) { _rotation += rotation; }
    const Vector3& GetRotation() const { return _rotation; }

    const Vector3& GetLastFrameAcceleration() const { return _lastFrameAcceleration; }

    void ApplyForce(const Vector3& force) { _accumulatedForce += force; _isAwake = true; }
    void ApplyForceAtWorldPoint(const Vector3& force, const Vector3& point);
    void ApplyForceAtLocalPoint(const Vector3& force, const Vector3& point);

    void ApplyTorque(const Vector3& torque) { _accumulatedTorque += torque; _isAwake = true; }

    void ClearAccumulators() { _accumulatedForce.Clear(); _accumulatedTorque.Clear(); }

    Vector3 ToWorldPoint(const Vector3& point) const { return _transformMatrix.Transform(point); }
    Vector3 ToLocalPoint(const Vector3& point) const { return _transformMatrix.TransformInverse(point); }

    Vector3 ToWorldDirection(const Vector3& direction) const { return _transformMatrix.TransformDirection(direction); }
    Vector3 ToLocalDirection(const Vector3& direction) const { return _transformMatrix.TransformInverseDirection(direction); }

    void CanSleep(bool canSleep) { _canSleep = canSleep; }
    bool CanSleep() const { return _canSleep; }

    void IsAwake(bool isAwake) { _isAwake = isAwake; }
    bool IsAwake() const { return _isAwake; }

protected:
    /**
     * @brief The inverse of the body's inertia tensor.
     */
    Matrix3 _inverseInertiaTensor;

    real_t _linearDamping = 1.0f;
    real_t _angularDamping = 1.0f;

    Quaternion _orientation;

    /**
     * @brief Angular velocity.
     */
    Vector3 _rotation;

    // Below are the derived data, which are calculated from the above data.

    /**
     * @brief Inertia tensor in world space.
     */
    Matrix3 _inverseInertiaTensorWorld;

    /**
     * @brief Convert between body space and world space.
     */
    Matrix4 _transformMatrix;

    /**
     * @brief Accumulated force in the current simulation step.
     */
    Vector3 _accumulatedForce;

    /**
     * @brief Accumulated torque in the current simulation step.
     */
    Vector3 _accumulatedTorque;

    /**
     * @brief Acceleration of the body in the last frame.
     * @note
     * We need this because we want to get the position of the body without
     * collision to perform collision detection.
     */
    Vector3 _lastFrameAcceleration;

    /**
     * @brief The current motion energy of the body.
     * @note It can be used to determine if the body is at rest (so not to update).
     */
    real_t _motion = 0;

    /**
     * @brief Whether this body can sleep.
     */
    bool _canSleep = false;

    /**
     * @brief Whether this body is awake.
     */
    bool _isAwake = true;
};

// clang-format on

/**
 * @brief If the motion of the body is below this value, it will be put to sleep.
 */
void SetSleepEpsilon(real_t epsilon);
real_t GetSleepEpsilon();

DPHX_END
