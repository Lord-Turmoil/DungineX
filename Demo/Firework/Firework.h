#pragma once

#include <DgeX/DgeX.h>

using namespace DgeX;
using namespace DgeX::Physics;

class Firework : public Particle, public PooledObject<Firework>
{
public:
    void SetType(int type)
    {
        _type = type;
    }

    int GetType() const
    {
        return _type;
    }

    void SetIteration(int iteration)
    {
        _iteration = iteration;
    }

    int GetIteration() const
    {
        return _iteration;
    }

    void ResetIteration()
    {
        _iteration = 0;
    }

    void SetAge(real_t age)
    {
        _age = age;
    }

    real_t GetAge() const
    {
        return _age;
    }

    /**
     * @brief Update particle status.
     * @param delta elapsed time
     * @return true if the particle need to be removed
     */
    bool Update(real_t delta)
    {
        // Update our physical state
        Integrate(delta);

        // We work backwards from our age to zero.
        _age -= delta;
        return _age < 0;
    }

private:
    int _type = 0;      // type of the firework
    int _iteration = 0; // the iteration of the firework, payload adds one to this
    real_t _age = 0;    // time to live
};

// Type and count of new particle
struct FireworkPayload
{
    int Type;
    int Count;
};

class FireworkSpecification
{
public:
    int Type;

    real_t MinAge; // minimum time to live
    real_t MaxAge; // maximum time to live

    real_t Damping; // damping factor

    Vector3 MinVelocity; // minimum velocity on create
    Vector3 MaxVelocity; // maximum velocity on create

    Vector3 Gravity = Vector3(0, -9.81, 0); // gravity

    std::vector<FireworkPayload> Payloads; // payloads to create new particles

public:
    void AddPayload(FireworkPayload payload)
    {
        Payloads.push_back(payload);
    }

    void AddPayload(int type, int count)
    {
        AddPayload({ type, count });
    }

    void Create(Firework* firework, Firework* parent) const;
    void Create(Firework* firework, const Vector3& position) const;

    void SetParameters(int type, real_t minAge, real_t maxAge, const Vector3& minVelocity, const Vector3& maxVelocity,
                       real_t damping)
    {
        Type = type;
        MinAge = minAge;
        MaxAge = maxAge;
        MinVelocity = minVelocity;
        MaxVelocity = maxVelocity;
        Damping = damping;
    }

private:
    void _Create(Firework* firework, const Vector3* position, const Vector3* velocity) const;
};
