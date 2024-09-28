#pragma once

#include "DgeX/Physics/Particle/ParticleContact.h"

DPHX_BEGIN

class Particle;

/**
 * @brief Base class for anchored particle link.
 */
class ParticleAnchoredLink : public ParticleContactGenerator
{
public:
    ParticleAnchoredLink() : _particle(nullptr), _anchor(nullptr)
    {
    }

    ParticleAnchoredLink(Particle* particle, Vector3* anchor) : _particle(particle), _anchor(anchor)
    {
    }

    ~ParticleAnchoredLink() override = default;

    // clang-format off
    void SetParticle(Particle* particle) { _particle = particle; }
    Particle* GetParticle() const { return _particle; }

    void SetAnchor(Vector3* anchor) { _anchor = anchor; }
    Vector3* GetAnchor() const { return _anchor; }

    // clang-format on
protected:
    real_t _Length() const;

protected:
    Particle* _particle;
    Vector3* _anchor;
};

/**
 * @brief Restricts the maximum distance between two particles.
 */
class ParticleAnchoredCable : public ParticleAnchoredLink
{
public:
    ParticleAnchoredCable() : _maxLength(0), _restitution(0)
    {
    }

    ParticleAnchoredCable(Particle* particle, Vector3* anchor, real_t maxLength, real_t restitution)
        : ParticleAnchoredLink(particle, anchor), _maxLength(maxLength), _restitution(restitution)
    {
    }

    ~ParticleAnchoredCable() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetRestitution(real_t restitution) { _restitution = restitution; }
    real_t GetMaxLength() const { return _maxLength; }

    void SetMaxLength(real_t maxLength) { _maxLength = maxLength; }
    real_t GetRestitution() const { return _restitution; }

    // clang-format on
private:
    real_t _maxLength;
    real_t _restitution;
};

/**
 * @brief Half rod restricts the minimum distance between two particles.
 */
class ParticleAnchoredHalfRod : public ParticleAnchoredLink
{
public:
    ParticleAnchoredHalfRod() : _minLength(0), _restitution(0)
    {
    }

    ParticleAnchoredHalfRod(Particle* particle, Vector3* anchor, real_t minLength, real_t restitution)
        : ParticleAnchoredLink(particle, anchor), _minLength(minLength), _restitution(restitution)
    {
    }

    ~ParticleAnchoredHalfRod() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetRestitution(real_t restitution) { _restitution = restitution; }
    real_t GetMinLength() const { return _minLength; }

    void SetMinLength(real_t minLength) { _minLength = minLength; }
    real_t GetRestitution() const { return _restitution; }

    // clang-format on
private:
    real_t _minLength;
    real_t _restitution;
};

/**
 * @brief Restricts the distance between two particles.
 */
class ParticleAnchoredRod : public ParticleAnchoredLink
{
public:
    ParticleAnchoredRod() : _length(0)
    {
    }

    ParticleAnchoredRod(Particle* particle, Vector3* anchor, real_t length)
        : ParticleAnchoredLink(particle, anchor), _length(length)
    {
    }

    ~ParticleAnchoredRod() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetLength(real_t length) { _length = length; }
    real_t GetLength() const { return _length; }

    // clang-format on

private:
    real_t _length;
};

/**
 * @brief Base class for particle link.
 */
class ParticleLink : public ParticleContactGenerator
{
public:
    ParticleLink() : _particles{ nullptr, nullptr }
    {
    }

    ParticleLink(Particle* first, Particle* second) : _particles{ first, second }
    {
    }

    ~ParticleLink() override = default;

    // clang-format off
    void SetFirst(Particle* particle) { _particles[0] = particle; }
    Particle* GetFirst() const { return _particles[0]; }

    void SetSecond(Particle* particle) { _particles[1] = particle; }
    Particle* GetSecond() const { return _particles[1]; }

    // clang-format on
protected:
    real_t _Length() const;

protected:
    Particle* _particles[2];
};

/**
 * @brief Restricts the maximum distance between two particles.
 */
class ParticleCable : public ParticleLink
{
public:
    ParticleCable() : _maxLength(0), _restitution(0)
    {
    }

    ParticleCable(Particle* first, Particle* second, real_t maxLength, real_t restitution)
        : ParticleLink(first, second), _maxLength(maxLength), _restitution(restitution)
    {
    }

    ~ParticleCable() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetRestitution(real_t restitution) { _restitution = restitution; }
    real_t GetMaxLength() const { return _maxLength; }

    void SetMaxLength(real_t maxLength) { _maxLength = maxLength; }
    real_t GetRestitution() const { return _restitution; }

    // clang-format on
private:
    real_t _maxLength;
    real_t _restitution;
};

/**
 * @brief Restricts the minimum distance between two particles.
 */
class ParticleHalfRod : public ParticleLink
{
public:
    ParticleHalfRod() : _minLength(0), _restitution(0)
    {
    }

    ParticleHalfRod(Particle* first, Particle* second, real_t minLength, real_t restitution)
        : ParticleLink(first, second), _minLength(minLength), _restitution(restitution)
    {
    }

    ~ParticleHalfRod() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetRestitution(real_t restitution) { _restitution = restitution; }
    real_t GetMinLength() const { return _minLength; }

    void SetMinLength(real_t minLength) { _minLength = minLength; }
    real_t GetRestitution() const { return _restitution; }

    // clang-format on
private:
    real_t _minLength;
    real_t _restitution;
};

/**
 * @brief Restricts the distance between two particles.
 */
class ParticleRod : public ParticleLink
{
public:
    ParticleRod() : _length(0)
    {
    }

    ParticleRod(Particle* first, Particle* second, real_t length) : ParticleLink(first, second), _length(length)
    {
    }

    ~ParticleRod() override = default;

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetLength(real_t length) { _length = length; }
    real_t GetLength() const { return _length; }

    // clang-format on
private:
    real_t _length;
};

/**
 * @brief
 * A panel that restricts the movement of a particle to a certain plane.
 * It can be used as ground.
 */
class ParticlePanel : public ParticleContactGenerator
{
public:
    ParticlePanel() : _particle(nullptr), _normal(Vector3::UnitY), _anchor(Vector3::Zero), _restitution(0.2f)
    {
    }

    ParticlePanel(Particle* particle, const Vector3& normal, const Vector3& anchor, real_t restitution = 0.2f)
        : _particle(particle), _normal(normal), _anchor(anchor), _restitution(restitution)
    {
        _normal.Normalize(); // ensure we have a valid normal
    }

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetParticle(Particle* particle) { _particle = particle; }
    Particle* GetParticle() const { return _particle; }

    void SetNormal(const Vector3& normal) { _normal = normal; }
    const Vector3& GetNormal() const { return _normal; }

    void SetAnchor(const Vector3& anchor) { _anchor = anchor; }
    const Vector3& GetAnchor() const { return _anchor; }

    void SetRestitution(real_t restitution) { _restitution = restitution; }
    real_t GetRestitution() const { return _restitution; }

    // clang-format on
protected:
    Particle* _particle;

    Vector3 _normal;
    Vector3 _anchor;

    real_t _restitution;
};

/**
 * @brief Apply panel constraint to multiple particles.
 */
class ParticlesPanel : public ParticleContactGenerator
{
public:
    ParticlesPanel() : _particles(nullptr), _count(0), _normal(Vector3::UnitY), _restitution(0.1f)
    {
    }

    ParticlesPanel(Particle* particles, size_t count, const Vector3& normal, const Vector3& anchor,
                   real_t restitution = 0.1f)
        : _particles(particles), _count(count), _normal(normal), _anchor(anchor), _restitution(restitution)
    {
    }

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetParticles(Particle* particles, size_t count) { _particles = particles; _count = count; }
    Particle* GetParticles() const { return _particles; }
    size_t GetCount() const { return _count; }

    void SetNormal(const Vector3& normal) { _normal = normal; _normal.Normalize(); }
    const Vector3& GetNormal() const { return _normal; }

    void SetAnchor(const Vector3& anchor) { _anchor = anchor; }
    const Vector3& GetAnchor() const { return _anchor; }

    void SetRestitution(real_t restitution) { _restitution = restitution; }
    real_t GetRestitution() const { return _restitution; }

    // clang-format on
protected:
    Particle* _particles;
    size_t _count;

    Vector3 _normal;
    Vector3 _anchor;

    real_t _restitution;
};

/**
 * @brief
 * A segment that restricts the movement of a particle to a certain line.
 * Instead of a plane, it restricts the movement to a limited segment.
 */
class ParticleSegment : public ParticleContactGenerator
{
public:
    ParticleSegment() : _particle(nullptr), _start(Vector3::Zero), _end(Vector3::UnitX), _restitution(0.2f)
    {
        _UpdateDirection();
    }

    ParticleSegment(Particle* particle, const Vector3& start, const Vector3& end, real_t restitution = 0.2f)
        : _particle(particle), _start(start), _end(end), _restitution(restitution)
    {
        _UpdateDirection();
    }

    uint32_t AddContact(ParticleContact* contact, uint32_t limit) const override;

    // clang-format off
    void SetParticle(Particle* particle) { _particle = particle; }
    Particle* GetParticle() const { return _particle; }

    void SetStart(const Vector3& start) { _start = start; _UpdateDirection(); }
    const Vector3& GetStart() const { return _start; }

    void SetEnd(const Vector3& end) { _end = end; _UpdateDirection(); }
    const Vector3& GetEnd() const { return _end; }

    void SetRestitution(real_t restitution) { _restitution = restitution; }
    real_t GetRestitution() const { return _restitution; }

    void SetRadius(real_t radius) { _radius = radius; }
    real_t GetRadius() const { return _radius; }

    // clang-format on

private:
    void _UpdateDirection()
    {
        _direction = _end - _start;
    }

protected:
    Particle* _particle;

    Vector3 _start;
    Vector3 _end;
    Vector3 _direction;

    real_t _restitution;

    /**
     * @brief Radius of the particle.
     */
    real_t _radius;
};

DPHX_END
