#pragma once

#include "DgeX/Physics/Particle/Particle.h"

DPHX_BEGIN

class ParticleForceGenerator
{
public:
    virtual ~ParticleForceGenerator() = default;

    /**
     * @brief Update force on a particle.
     * @param particle apply force on this particle
     * @param delta delta time since last update
     */
    virtual void UpdateForce(Particle* particle, real_t delta) = 0;
};

/**
 * @brief To apply gravity force on a particle.
 * @note
 * Although particles can have intrinsic acceleration, using a force generator
 * can be more flexible.
 */
class ParticleGravity : public ParticleForceGenerator
{
public:
    ParticleGravity(const Vector3& gravity) : _gravity(gravity)
    {
    }

    ~ParticleGravity() override = default;

    void UpdateForce(Particle* particle, real_t delta) override;

    // clang-format off
    void SetGravity(const Vector3& gravity) { _gravity = gravity; }
    const Vector3& GetGravity() const { return _gravity; }

    // clang-format on

protected:
    /**
     * @brief The gravity acceleration.
     */
    Vector3 _gravity;
};

/**
 * @brief To apply drag force on a particle.
 * @note
 * Although particles can have intrinsic drag (damping), using a force generator
 * can a fine-grained control.
 */
class ParticleDrag : public ParticleForceGenerator
{
public:
    ParticleDrag(real_t k1, real_t k2);
    ~ParticleDrag() override = default;

    void UpdateForce(Particle* particle, real_t delta) override;

protected:
    real_t _k1;
    real_t _k2;
};

/**
 * @brief A spring (spring, bungee) that has one end attached to a fixed point.
 */
class ParticleAnchoredSpring : public ParticleForceGenerator
{
public:
    ParticleAnchoredSpring(Vector3* anchor, real_t springConstant, real_t restLength)
        : _anchor(anchor), _springConstant(springConstant), _restLength(restLength)
    {
    }

    ~ParticleAnchoredSpring() override = default;

    // clang-format off
    void SetAnchor(Vector3* anchor) { _anchor = anchor; }
    Vector3* GetAnchor() const { return _anchor; }

    void SetSpringConstant(real_t springConstant) { _springConstant = springConstant; }
    real_t GetSpringConstant() const { return _springConstant; }

    void SetRestLength(real_t restLength) { _restLength = restLength; }
    real_t GetRestLength() const { return _restLength; }

    // clang-format on

    void UpdateForce(Particle* particle, real_t delta) override;

protected:
    /**
     * @note Here we use pointer so that changing the anchor position outside will
     * automatically update all anchors.
     */
    Vector3* _anchor;

    real_t _springConstant;
    real_t _restLength;
};

/**
 * @brief A bungee can be a special spring that doesn't expand.
 */
class ParticleAnchoredBungee : public ParticleAnchoredSpring
{
public:
    ParticleAnchoredBungee(Vector3* anchor, real_t springConstant, real_t restLength)
        : ParticleAnchoredSpring(anchor, springConstant, restLength)
    {
    }

    void UpdateForce(Particle* particle, real_t delta) override;
};

/**
 * @brief
 * Fake spring is to handle hard constraints, it uses Simple Harmonic Motion
 * to calculate the position. Therefore, it has to be connected to a fixed
 * location.
 */
class ParticleFakeSpring : public ParticleForceGenerator
{
public:
    ParticleFakeSpring(Vector3* anchor, real_t springConstant, real_t damping)
        : _anchor(anchor), _springConstant(springConstant), _damping(damping)
    {
    }

    ~ParticleFakeSpring() override = default;

    // clang-format off
    void SetAnchor(Vector3* anchor) { _anchor = anchor; }
    Vector3* GetAnchor() const { return _anchor; }

    void SetSpringConstant(real_t springConstant) { _springConstant = springConstant; }
    real_t GetSpringConstant() const { return _springConstant; }

    void SetDamping(real_t damping) { _damping = damping; }
    real_t GetDamping() const { return _damping; }

    // clang-format on

    void UpdateForce(Particle* particle, real_t delta) override;

protected:
    Vector3* _anchor;
    real_t _springConstant;
    real_t _damping;
};

/**
 * @brief A spring that connects two particles.
 * @note
 * It is a one-way binding, so... you have to create two force generator
 * to apply the force on both particles.
 */
class ParticleSpring : public ParticleForceGenerator
{
public:
    ParticleSpring(Particle* other, real_t springConstant, real_t restLength)
        : _other(other), _springConstant(springConstant), _restLength(restLength)
    {
    }

    ~ParticleSpring() override = default;

    // clang-format off
    void SetOther(Particle* other) { _other = other; }
    Particle* GetOther() const { return _other; }

    void SetSpringConstant(real_t springConstant) { _springConstant = springConstant;     }
    real_t GetSpringConstant() const { return _springConstant; }

    void SetRestLength(real_t restLength) { _restLength = restLength; }
    real_t GetRestLength() const { return _restLength; }

    // clang-format on

    void UpdateForce(Particle* particle, real_t delta) override;

protected:
    Particle* _other;
    real_t _springConstant;
    real_t _restLength;
};

/**
 * @brief Well, a bungee is a spring that doesn't expand.
 */
class ParticleBungee : public ParticleSpring
{
public:
    ParticleBungee(Particle* other, real_t springConstant, real_t restLength)
        : ParticleSpring(other, springConstant, restLength)
    {
    }

    void UpdateForce(Particle* particle, real_t delta) override;
};

/**
 * @brief Holds a list of force generators and the particles they apply to.
 */
class ParticleForceRegistry
{
public:
    void Add(Particle* particle, ParticleForceGenerator* forceGenerator);
    void Remove(Particle* particle, ParticleForceGenerator* forceGenerator);
    void Clear();

    void UpdateForce(real_t delta) const;

protected:
    struct ParticleForceRegistration
    {
        Particle* Particle;
        ParticleForceGenerator* ForceGenerator;

        ParticleForceRegistration(DPHX Particle* particle, ParticleForceGenerator* forceGenerator)
            : Particle(particle), ForceGenerator(forceGenerator)
        {
        }

        void UpdateForce(real_t delta) const
        {
            ForceGenerator->UpdateForce(Particle, delta);
        }
    };

    std::vector<ParticleForceRegistration> _registrations;
};

DPHX_END
