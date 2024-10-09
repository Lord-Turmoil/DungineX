#pragma once

#include <DgeX/DgeX.h>
#include <deque>

#include "DgeX/Physics/Particle/Contact/ParticleContact.h"

// The car. :)
class Car
{
    friend class Map;
    friend class DustController;

public:
    Car();
    ~Car();

    /**
     * @brief
     * Step on the gas pedal. threshold is the percentage of the maximum power.
     * It should be in the range of [0, 1].
     *
     * @warning
     * It should be called after rotation, as it will correct the car's direction.
     */
    void Forward(DgeX::Physics::real_t threshold);
    void Backward(DgeX::Physics::real_t threshold);

    void RotateLeft(DgeX::Physics::real_t threshold);
    void RotateRight(DgeX::Physics::real_t threshold);

    void Jump() const;
    void StopJump() const;

    // Reset all calculated states applied to the car.
    void ResetState();

    void Reset(const DgeX::Physics::Vector3& center);
    void MoveTo(const DgeX::Physics::Vector3& center);
    void Translate(const DgeX::Physics::Vector3& offset);

    // Register itself to the physics world.
    void Register(DgeX::Physics::ParticleWorld& world);
    // Unregister itself from the physics world.
    void Unregister(DgeX::Physics::ParticleWorld& world);

    void OnUpdate(DgeX::Physics::real_t delta);
    void OnRender() const;

    const DgeX::Physics::Vector3& GetCenter() const
    {
        return _center;
    }

    DgeX::Physics::real_t GetSpeed() const
    {
        return _body[0].GetVelocity().Magnitude();
    }

private:
    // Maximum engine force.
    static DgeX::Physics::real_t _sGasPower;
    static DgeX::Physics::real_t _sRotatePower;

    DgeX::Physics::real_t _wheelRadian[2];
    DgeX::Physics::real_t _desiredRotation[2];
    DgeX::Physics::real_t _actualRotation[2];

    bool _registered = false;

private:
    // below are physics properties

    void _Init(const DgeX::Physics::Vector3& center);
    void _UpdateCenter();

    DgeX::Physics::Vector3 _center;

    DgeX::Physics::Particle* _body;
    DgeX::Physics::Particle* _wheel;
    DgeX::Physics::ParticleRod* _frame;
    DgeX::Physics::ParticleAbsorber* _absorber;
    DgeX::Physics::ParticleStop* _absorberMinConstraint;
    DgeX::Physics::ParticleCable* _absorberMaxConstraint;

    // The direction of the wheel contact.
    DgeX::Physics::Vector3 _wheelNormal[2];
    DgeX::Physics::ParticleForce _wheelForce[2];

    // The force to rotate the car.
    DgeX::Physics::ParticleForce _rotateForce[2];

    DgeX::Physics::ParticleDrag _bodyDrag;
    DgeX::Physics::ParticleDrag _wheelDrag;

    // Car body and wheel offset from the center of mass
    static DgeX::Physics::Vector3 _sCenterOffset;
    static DgeX::Physics::Vector3 _sBodyOffset[6];
    static DgeX::Physics::Vector3 _sWheelOffset[2];

    static DgeX::Physics::real_t _sWheelRadius;

    static DgeX::Physics::real_t _sWheelMass;
    static DgeX::Physics::real_t _sBodyMass;
    static DgeX::Physics::real_t _sFrameMass;
    static DgeX::Physics::real_t _sTotalMass;

    static DgeX::Physics::real_t _sSpringConstant;
    static DgeX::Physics::real_t _sSpringDamping;
    static DgeX::Physics::real_t _sSpringRestLength;
};

/**
 * @brief The dust effect.
 */
class Dust : public DgeX::Physics::Particle, public DgeX::PooledObject<Dust>
{
public:
    void OnUpdate(DgeX::Physics::real_t delta);
    void OnRender();

    void Init(DgeX::Physics::real_t timeToLive, float size);

    bool IsDead() const
    {
        return _elapsed > _timeToLive;
    }

private:
    DgeX::Physics::real_t _timeToLive = 0.0;
    DgeX::Physics::real_t _elapsed = 0.0;
    float _size = 0.1f;
    float _alpha = 1.0f;
};

class DustController
{
public:
    DustController(uint32_t capacity) : _pool(capacity)
    {
    }

    void OnUpdate(DgeX::Physics::real_t delta);
    void OnRender();

    void SetCar(Car* car)
    {
        _car = car;
    }

    void SetWorld(DgeX::Physics::ParticleWorld* world)
    {
        _world = world;
    }

private:
    void _Create(const DgeX::Physics::Vector3& position, const DgeX::Physics::Vector3& velocity);

private:
    DgeX::ObjectPool<Dust> _pool;
    DgeX::ObjectQueue<Dust> _dusts;
    Car* _car = nullptr;
    DgeX::Physics::ParticleWorld* _world = nullptr;

    DgeX::Physics::real_t _elapsed = 0.0;

    DgeX::Physics::real_t _minTimeToLive = 0.3;
    DgeX::Physics::real_t _maxTimeToLive = 0.9;
    DgeX::Physics::real_t _minVelocity = 0.5;
    DgeX::Physics::real_t _maxVelocity = 10.0;
};

/**
 * @brief
 * This class provide an extra callback function to set the wheel's
 * contact direction.
 */
class Map : public DgeX::Physics::ParticleContactGenerator
{
public:
    void SetCar(Car* car)
    {
        _car = car;
    }

    uint32_t AddContact(DgeX::Physics::ParticleContact* contact, uint32_t limit) const override;

    void PushBack(const DgeX::Physics::Vector3& vertex)
    {
        _vertices.push_back(vertex);
    }

    void PushFront(const DgeX::Physics::Vector3& vertex)
    {
        _vertices.push_front(vertex);
    }

    void PopBack()
    {
        _vertices.pop_back();
    }

    void PopFront()
    {
        _vertices.pop_front();
    }

    void Clear()
    {
        _vertices.clear();
    }

    void OnRender();

private:
    Car* _car = nullptr;
    std::deque<DgeX::Physics::Vector3> _vertices;
    DgeX::Physics::real_t _restitution = 0.1;
};

class CarController
{
public:
    void OnUpdate(DgeX::DeltaTime delta);

    void SetCar(Car* car)
    {
        _car = car;
    }

private:
    void _Forward(DgeX::Physics::real_t delta);
    void _Backward(DgeX::Physics::real_t delta);
    void _RotateLeft(DgeX::Physics::real_t delta);
    void _RotateRight(DgeX::Physics::real_t delta);
    void _Idle(DgeX::Physics::real_t delta);

private:
    Car* _car = nullptr;

    bool _forward = true;
    DgeX::Physics::real_t _gasTime = 0;
    bool _left = true;
    DgeX::Physics::real_t _rotateTime = 0;
    bool _jumping = false;
    DgeX::Physics::real_t _jumpTime = 0;

    // The time to reach the maximum gas and rotate.
    static DgeX::Physics::real_t _sGasTime;
    static DgeX::Physics::real_t _sRotateTime;
    static DgeX::Physics::real_t _sJumpTime;
};
