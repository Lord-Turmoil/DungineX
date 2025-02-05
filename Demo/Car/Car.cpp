#include "Car.h"

using namespace DgeX;
using namespace DgeX::Physics;
using namespace DgeX::Utils::Easing;

static real_t Distance(const Particle& p0, const Particle& p1)
{
    return (p0.GetPosition() - p1.GetPosition()).Magnitude();
}

// clang-format off
Vector3 Car::_sCenterOffset = Vector3(2.4, 1.0);

Vector3 Car::_sBodyOffset[6] = {
    Vector3(1.8, 2.1) - _sCenterOffset,
    Vector3(3.0, 2.1) - _sCenterOffset,
    Vector3(3.0, 1.0) - _sCenterOffset,
    Vector3(1.8, 1.0) - _sCenterOffset,
    Vector3(0.6, 1.4) - _sCenterOffset,
    Vector3(4.2, 1.4) - _sCenterOffset
};

Vector3 Car::_sWheelOffset[2] = {
    Vector3(0.6, 0.55) - _sCenterOffset,
    Vector3(4.2, 0.55) - _sCenterOffset
};

real_t Car::_sWheelRadius  = 0.55;
real_t Car::_sWheelMass    = 50;
real_t Car::_sBodyMass     = 500;
real_t Car::_sFrameMass    = 100;
real_t Car::_sTotalMass    = _sWheelMass * 2 + _sBodyMass * 2 + _sFrameMass * 4;

real_t Car::_sSpringConstant = 40000.0;
real_t Car::_sSpringDamping  = 8000.0;
real_t Car::_sSpringRestLength = (_sBodyOffset[4] - _sWheelOffset[0]).Magnitude() + 0.3;

const real_t MAX_GAS_POWER = 50000.0;
const real_t NORMAL_GAS_POWER = 15000.0;

real_t Car::_sGasPower    = 15000.0;
real_t Car::_sRotatePower = 12000.0;

// clang-format on

Car::Car()
{
    _body = new Particle[6];
    _wheel = new Particle[2];
    _frame = new ParticleRod[12];
    _absorber = new ParticleAbsorber[4];
    _absorberMinConstraint = new ParticleStop[2];
    _absorberMaxConstraint = new ParticleCable[2];
    _forceRegistration = new ParticleForceRegistration[16];

    _Init(Vector3::Zero);
}

Car::~Car()
{
    delete[] _body;
    delete[] _wheel;
    delete[] _frame;
    delete[] _absorber;
    delete[] _absorberMinConstraint;
    delete[] _absorberMaxConstraint;
    delete[] _forceRegistration;
}

void Car::Forward(real_t threshold)
{
    _wheelForce[0].Force = _wheelNormal[0] * _sGasPower * threshold;
    _wheelForce[1].Force = _wheelNormal[1] * _sGasPower * threshold;

    if (!_wheelNormal[0].IsZero())
    {
        Vector3 direction = _body[5].GetPosition() - _body[4].GetPosition();
        Vector3 normal = Vector3(-direction.Y, direction.X).Normalized();
        Vector3 correctedRotation = normal * _sRotatePower * threshold * threshold * 0.2;
        _rotateForce[1].Force = _rotateForce[1].Force + correctedRotation;
    }
    else
    {
        _desiredRotation[0] = _sGasPower / _sTotalMass / _sWheelRadius * threshold;
    }

    if (_wheelNormal[1].IsZero())
    {
        _desiredRotation[1] = _sGasPower / _sTotalMass / _sWheelRadius * threshold;
    }
}

void Car::Backward(real_t threshold)
{
    _wheelForce[0].Force = _wheelNormal[0] * _sGasPower * -threshold;
    _wheelForce[1].Force = _wheelNormal[1] * _sGasPower * -threshold;

    if (!_wheelNormal[1].IsZero())
    {
        Vector3 direction = _body[4].GetPosition() - _body[5].GetPosition();
        Vector3 normal = Vector3(direction.Y, -direction.X).Normalized();
        Vector3 correctedRotation = normal * _sRotatePower * threshold * threshold * 0.2;
        _rotateForce[0].Force = _rotateForce[0].Force + correctedRotation;
    }
    else
    {
        _desiredRotation[1] = -_sGasPower / _sTotalMass / _sWheelRadius * threshold;
    }

    if (_wheelNormal[0].IsZero())
    {
        _desiredRotation[0] = -_sGasPower / _sTotalMass / _sWheelRadius * threshold;
    }
}

void Car::RotateLeft(real_t threshold)
{
    Vector3 direction = _body[5].GetPosition() - _body[4].GetPosition();
    Vector3 normal = Vector3(-direction.Y, direction.X).Normalized();
    _rotateForce[1].Force = normal * _sRotatePower * threshold;
}

void Car::RotateRight(real_t threshold)
{
    Vector3 direction = _body[4].GetPosition() - _body[5].GetPosition();
    Vector3 normal = Vector3(direction.Y, -direction.X).Normalized();
    _rotateForce[0].Force = normal * _sRotatePower * threshold;
}

void Car::Jump() const
{
    for (int i = 2; i < 4; i++)
    {
        _absorber[i].SpringConstant = _sSpringConstant * 1.2;
        _absorber[i].RestLength = _sSpringRestLength * 1.2;
        _absorber[i].Damping = 0.0;
    }
}

void Car::StopJump() const
{
    for (int i = 2; i < 4; i++)
    {
        _absorber[i].SpringConstant = _sSpringConstant;
        _absorber[i].RestLength = _sSpringRestLength;
        _absorber[i].Damping = _sSpringDamping;
    }
}

void Car::ResetState()
{
    _wheelForce[0].Force = Vector3::Zero;
    _wheelForce[1].Force = Vector3::Zero;

    _rotateForce[0].Force = Vector3::Zero;
    _rotateForce[1].Force = Vector3::Zero;
}

void Car::Reset(const Vector3& center)
{
    _Init(center);
    _UpdateCenter();
    _wheelRadian[0] = _wheelRadian[1] = 0;
    _desiredRotation[0] = _desiredRotation[1] = 0;
    _actualRotation[0] = _actualRotation[1] = 0;
}

void Car::MoveTo(const Vector3& center)
{
    _UpdateCenter();
    Translate(center - _center);
}

void Car::Translate(const Vector3& offset)
{
    for (int i = 0; i < 2; i++)
    {
        _wheel[i].Translate(offset);
    }
    for (int i = 0; i < 4; i++)
    {
        _body[i].Translate(offset);
    }
}

void Car::Register(ParticleWorld& world)
{
    if (_registered)
    {
        return;
    }

    for (int i = 0; i < 2; i++)
    {
        world.AddParticle(&_wheel[i]);
    }
    for (int i = 0; i < 6; i++)
    {
        world.AddParticle(&_body[i]);
    }

    auto& forceRegistry = world.GetForceRegistry();

    _forceRegistration[0] = ParticleForceRegistration(&_body[2], &_wheelForce[0]);
    _forceRegistration[1] = ParticleForceRegistration(&_body[3], &_wheelForce[1]);
    _forceRegistration[2] = ParticleForceRegistration(&_body[4], &_rotateForce[0]);
    _forceRegistration[3] = ParticleForceRegistration(&_body[5], &_rotateForce[1]);

    for (int i = 0; i < 6; i++)
    {
        _forceRegistration[4 + i] = ParticleForceRegistration(&_body[i], &_bodyDrag);
    }
    for (int i = 0; i < 2; i++)
    {
        _forceRegistration[10 + i * 2] = ParticleForceRegistration(&_wheel[i], &_absorber[i]);
        _forceRegistration[11 + i * 2] = ParticleForceRegistration(&_wheel[i], &_wheelDrag);
    }
    for (int i = 0; i < 2; i++)
    {
        _forceRegistration[14 + i] = ParticleForceRegistration(&_body[4 + i], &_absorber[2 + i]);
    }

    for (int i = 0; i < 16; i++)
    {
        forceRegistry.Add(&_forceRegistration[i]);
    }

    auto& contactRegistry = world.GetContactRegistry();
    for (int i = 0; i < 12; i++)
    {
        contactRegistry.Add(&_frame[i]);
    }
    for (int i = 0; i < 2; i++)
    {
        contactRegistry.Add(&_absorberMinConstraint[i]);
        contactRegistry.Add(&_absorberMaxConstraint[i]);
    }

    _registered = true;
}

void Car::Unregister(ParticleWorld& world)
{
    if (!_registered)
    {
        return;
    }

    for (int i = 0; i < 2; i++)
    {
        world.RemoveParticle(&_wheel[i]);
    }
    for (int i = 0; i < 6; i++)
    {
        world.RemoveParticle(&_body[i]);
    }

    auto& forceRegistry = world.GetForceRegistry();
    for (int i = 0; i < 16; i++)
    {
        forceRegistry.Remove(&_forceRegistration[i]);
    }

    auto& contactRegistry = world.GetContactRegistry();
    for (int i = 0; i < 12; i++)
    {
        contactRegistry.Remove(&_frame[i]);
    }
    for (int i = 0; i < 2; i++)
    {
        contactRegistry.Remove(&_absorberMinConstraint[i]);
        contactRegistry.Remove(&_absorberMaxConstraint[i]);
    }

    _registered = false;
}

void Car::OnUpdate(real_t delta)
{
    using namespace DgeX::Utils::Easing;

    for (int i = 0; i < 2; i++)
    {
        if (_wheelNormal[i].IsZero())
        {
            if (_desiredRotation[i] == 0.0)
            {
                _actualRotation[i] *= 1 - 0.9 * delta;
            }
            else
            {
                _actualRotation[i] = Interpolate(_actualRotation[i], _desiredRotation[i], 0.1, Linear);
            }
        }
        else
        {
            real_t desired = _wheel[i].GetVelocity().Magnitude() / 0.55;
            if (_wheel[i].GetVelocity() * _wheelNormal[i] < 0.0)
            {
                desired = -desired;
            }
            _actualRotation[i] = Interpolate(_actualRotation[i], desired, 0.5, EaseOutQuad);
        }
        _wheelRadian[i] += _actualRotation[i] * delta;
    }

    _desiredRotation[0] = _desiredRotation[1] = 0;

    // check wheel
    Vector3 v1 = _wheel[1].GetPosition() - _body[2].GetPosition();
    Vector3 v2 = _body[5].GetPosition() - _body[2].GetPosition();
    if ((v1 % v2).Z < 0)
    {
        // let v1 = v2 rotate 10 degree
        real_t angle = -Math::PI<real_t> / 18.0;
        real_t cos = Math::Cos(angle);
        real_t sin = Math::Sin(angle);
        Vector3 newV1 = Vector3(v2.X * cos - v2.Y * sin, v2.X * sin + v2.Y * cos, 0);
        _wheel[1].SetPosition(_body[2].GetPosition() + newV1);
    }

    v1 = _wheel[0].GetPosition() - _body[3].GetPosition();
    v2 = _body[4].GetPosition() - _body[3].GetPosition();
    if ((v1 % v2).Z > 0)
    {
        // let v1 = v2 rotate -10 degree
        real_t angle = Math::PI<real_t> / 18.0;
        real_t cos = Math::Cos(angle);
        real_t sin = Math::Sin(angle);
        Vector3 newV1 = Vector3(v2.X * cos - v2.Y * sin, v2.X * sin + v2.Y * cos, 0);
        _wheel[0].SetPosition(_body[3].GetPosition() + newV1);
    }

    _UpdateCenter();
}

void Car::OnRender() const
{
    auto wheelColor = Color::FromRGBA(0xFF57553E).ToVec4();
    auto wheelRodColor = Color::FromRGBA(0xFF27241B).ToVec4();
    auto frameColor = Color::FromRGBA(0xFF686D4D).ToVec4();
    auto pointColor = Color::DarkGray.ToVec4();
    auto springColor = Color::LightGray.ToVec4();

    // RenderApi::SetLineWidth(4.f);
    for (int i = 0; i < 12; i++)
    {
        auto start = _frame[i].First()->GetPosition().ToGlmVec3();
        auto end = _frame[i].Second()->GetPosition().ToGlmVec3();
        RenderApi::DrawLine(start, end, frameColor);
    }

    // RenderApi::SetLineWidth(5.f);
    for (int i = 0; i < 2; i++)
    {
        auto start = _body[4 + i].GetPosition().ToGlmVec3();
        auto end = _wheel[i].GetPosition().ToGlmVec3();
        RenderApi::DrawLine(start, end, springColor);
    }

    for (int i = 0; i < 6; i++)
    {
        RenderApi::DrawFilledCircle(_body[i].GetPosition().ToGlmVec3(1.0f), 0.1f, pointColor);
    }

    // RenderApi::SetLineWidth(10.0f);
    for (int i = 0; i < 2; i++)
    {
        auto pos = _wheel[i].GetPosition().ToGlmVec3();
        glm::vec3 start = { -Math::Cos(_wheelRadian[i]), Math::Sin(_wheelRadian[i]), 0.f };
        glm::vec3 end = { Math::Cos(_wheelRadian[i]), -Math::Sin(_wheelRadian[i]), 0.f };
        start = start * static_cast<float>(_sWheelRadius) + pos;
        end = end * static_cast<float>(_sWheelRadius) + pos;
        RenderApi::DrawLine(start, end, wheelRodColor);
        RenderApi::DrawCircle(pos, static_cast<float>(_sWheelRadius), wheelColor);
        RenderApi::DrawFilledCircle({ pos.x, pos.y, 1.0f }, 0.1f, pointColor);
    }
}

void Car::_Init(const Vector3& center)
{
    Vector3 gravity = Vector3(0, -20.0, 0);

    for (int i = 0; i < 2; i++)
    {
        _wheel[i].SetPosition(center + _sWheelOffset[i]);
        _wheel[i].SetMass(_sWheelMass);
        _wheel[i].SetDamping(0.9);
        _wheel[i].SetAcceleration(gravity);
    }

    for (int i = 0; i < 6; i++)
    {
        _body[i].SetPosition(center + _sBodyOffset[i]);
        _body[i].SetMass(i == 2 || i == 3 ? _sBodyMass : _sFrameMass);
        _body[i].SetDamping(0.9);
        _body[i].SetAcceleration(gravity);
    }

    // main frame
    for (int i = 0; i < 4; i++)
    {
        _frame[i] = ParticleRod(&_body[i], &_body[(i + 1) % 4], Distance(_body[i], _body[(i + 1) % 4]));
    }

    // cross frame
    _frame[4] = ParticleRod(&_body[0], &_body[2], Distance(_body[0], _body[2]));
    _frame[5] = ParticleRod(&_body[1], &_body[3], Distance(_body[1], _body[3]));

    // wheel frame
    _frame[6] = ParticleRod(&_body[4], &_body[0], Distance(_body[4], _body[0]));
    _frame[7] = ParticleRod(&_body[4], &_body[3], Distance(_body[4], _body[3]));
    _frame[8] = ParticleRod(&_body[5], &_body[1], Distance(_body[5], _body[1]));
    _frame[9] = ParticleRod(&_body[5], &_body[2], Distance(_body[5], _body[2]));

    // wheel rods
    _frame[10] = ParticleRod(&_body[3], &_wheel[0], Distance(_body[3], _wheel[0]));
    _frame[11] = ParticleRod(&_body[2], &_wheel[1], Distance(_body[2], _wheel[1]));

    // spring
    for (int i = 0; i < 2; i++)
    {
        _absorber[i] = ParticleAbsorber(&_body[4 + i], _sSpringConstant, _sSpringRestLength, _sSpringDamping);
        _absorberMinConstraint[i] = ParticleStop(&_body[4 + i], &_wheel[i], _sSpringRestLength * 0.5, 0.4);
        _absorberMaxConstraint[i] = ParticleCable(&_body[4 + i], &_wheel[i], _sSpringRestLength * 1.2, 0.4);
    }
    for (int i = 0; i < 2; i++)
    {
        _absorber[2 + i] = ParticleAbsorber(&_wheel[i], _sSpringConstant, _sSpringRestLength, _sSpringDamping);
    }

    _bodyDrag = ParticleDrag(0.1, 0.1);
    _wheelDrag = ParticleDrag(0.05, 0.05);

    ResetState();
}

void Car::_UpdateCenter()
{
    _center = Vector3::Zero;
    for (int i = 0; i < 4; i++)
    {
        _center += _body[i].GetPosition();
    }
    _center *= 0.25;
}

/*
 * ===================================================================
 * ------------------------------- Dust ------------------------------
 * ===================================================================
 */

void Dust::OnUpdate(real_t delta)
{
    _elapsed += delta;
}

void Dust::OnRender()
{
    using namespace DgeX::Utils::Easing;
    static glm::vec4 color = Color::Brown.ToVec4();
    color.a = Interpolate(_alpha, 0.0f, RangeToStep(0.0, _timeToLive, _elapsed), EaseInQuad);
    RenderApi::DrawFilledRect(GetPosition().ToGlmVec3(1.0f), { _size, _size }, color);
}

void Dust::Init(real_t timeToLive, float size)
{
    _timeToLive = timeToLive;
    _elapsed = 0.0;
    _size = size;
    _alpha = DgeX::Utils::Random::RandomNumber(0.8f, 1.0f);
}

void DustController::OnUpdate(real_t delta)
{
    _dusts.Lock();
    for (auto dust : _dusts)
    {
        dust->OnUpdate(delta);
        if (dust->IsDead())
        {
            dust->Dispose();
            _dusts.Remove(dust);
            _world->RemoveParticle(dust);
        }
    }
    _dusts.Unlock();

    _elapsed += delta;
    if (_elapsed > 0.001)
    {
        _elapsed = 0.0;
        for (int i = 0; i < 2; i++)
        {
            if (!_car->_wheelNormal[i].IsZero())
            {
                Vector3 pos = _car->_wheel[i].GetPosition() +
                              _car->_wheelNormal[i].RotatedZ(-Math::PI<real_t> / 2.0) * Car::_sWheelRadius;
                _Create(pos, _car->_wheel[i].GetVelocity());
            }
        }
    }
}

void DustController::OnRender()
{
    for (auto dust : _dusts)
    {
        dust->OnRender();
    }
}

void DustController::_Create(const Vector3& position, const Vector3& velocity)
{
    using namespace DgeX::Utils::Easing;
    static Vector3 minGravity = Vector3(0, -5.0, 0);
    static Vector3 maxGravity = Vector3(0, 1.0, 0);

    Vector3 horizontal = -_car->_wheelNormal[0];
    real_t speed = velocity.Magnitude();
    if (speed < 1.0)
    {
        return;
    }

    // int count = Interpolate(1, 3, RangeToStep(0.1, 20.0, speed), Linear);
    int count = 1;
    for (int i = 0; i < count; i++)
    {
        Dust* dust = _pool.TryAcquire();
        if (!dust)
        {
            DGEX_LOG_WARN("Dust pool is full");
            break;
        }

        real_t step = RangeToStep(1.0, 25.0, speed);
        float maxSize = Interpolate(0.1f, 0.5f, step, Linear);
        real_t maxVelocity = Interpolate(_minVelocity, _maxVelocity, step, Linear);
        real_t maxTTL = Interpolate(_minTimeToLive, _maxTimeToLive, step, Linear);
        double minRadian = Interpolate(-10.0, -40.0, step, Linear);
        Vector3 normal =
            horizontal.RotatedZ(DgeX::Utils::Random::RandomNumber(Math::ToRadians(-80.0), Math::ToRadians(minRadian)));

        normal *= DgeX::Utils::Random::RandomNumber(_minVelocity, maxVelocity);

        dust->SetPosition(position);
        dust->SetVelocity(normal);
        dust->SetAcceleration(Physics::Utils::RandomVector(minGravity, maxGravity));
        dust->SetMass(1.0);
        dust->SetDamping(0.9);
        dust->Init(DgeX::Utils::Random::RandomNumber(_minTimeToLive, maxTTL),
                   DgeX::Utils::Random::RandomNumber(0.05f, maxSize));

        _dusts.Add(dust);
        _world->AddParticle(dust);
    }
}

/*
 * ===================================================================
 * ------------------------------- Map -------------------------------
 * ===================================================================
 */

uint32_t Map::AddContact(ParticleContact* contact, uint32_t limit) const
{
    _car->_wheelNormal[0] = _car->_wheelNormal[1] = Vector3::Zero;

    uint32_t used = 0;
    for (int i = 0; i < 2 && used < limit; i++)
    {
        Particle* particle = &_car->_wheel[i];
        auto it = std::lower_bound(_vertices.begin(), _vertices.end(), particle->GetPosition(),
                                   [](const Vector3& a, const Vector3& b) { return a.X < b.X; });
        if (it == _vertices.end() || it == _vertices.begin())
        {
            continue;
        }

        int index = static_cast<int>(it - _vertices.begin());
        auto left = it - Math::Min(index, 3);
        auto right = it + Math::Min(static_cast<int>(_vertices.size()) - 1 - index, 2);

        for (it = left; it != right && used < limit; ++it)
        {
            Vector3 start = *it;
            Vector3 end = *(it + 1);
            Vector3 direction = end - start;

            Vector3 relativePosition = particle->GetPosition() - start;
            real_t projected = relativePosition * direction.Normalized();

            bool hasContact = false;

            if (projected <= 0)
            {
                // nearest to the start
                if (relativePosition.MagnitudeSquare() < Car::_sWheelRadius * Car::_sWheelRadius)
                {
                    contact->First(particle);
                    contact->Second(nullptr);
                    contact->ContactNormal = relativePosition.Normalized();
                    contact->Penetration = Car::_sWheelRadius - relativePosition.Magnitude();
                    contact->Restitution = _restitution;
                    contact++;
                    used++;
                    hasContact = true;
                }
            }
            else if (projected >= direction.Magnitude())
            {
                // nearest to the end
                relativePosition = particle->GetPosition() - end;
                if (relativePosition.MagnitudeSquare() < Car::_sWheelRadius * Car::_sWheelRadius)
                {
                    contact->First(particle);
                    contact->Second(nullptr);
                    contact->ContactNormal = relativePosition.Normalized();
                    contact->Penetration = Car::_sWheelRadius - relativePosition.Magnitude();
                    contact->Restitution = _restitution;
                    contact++;
                    used++;
                    hasContact = true;
                }
            }
            else
            {
                // nearest to the middle
                Vector3 normal(-direction.Y, direction.X);
                normal.Normalize();
                real_t penetration = relativePosition * normal;
                if (penetration - Car::_sWheelRadius < 0)
                {
                    Vector3 contactPoint = start + direction * (projected / direction.Magnitude());

                    contact->First(particle);
                    contact->Second(nullptr);
                    contact->ContactNormal = normal;
                    contact->Penetration = Car::_sWheelRadius - penetration;
                    contact->Restitution = _restitution;
                    contact++;
                    used++;
                    hasContact = true;
                }
            }

            if (hasContact)
            {
                _car->_wheelNormal[i] = (_car->_wheelNormal[i] + direction.Normalized()).Normalized();
            }
        }
    }

    return used;
}

void Map::OnRender()
{
    glm::vec4 color = Color::Black.ToVec4();
    RenderApi::SetLineWidth(5.0f);
    for (auto it = _vertices.begin(); it != _vertices.end(); ++it)
    {
        auto next = it + 1;
        if (next == _vertices.end())
        {
            break;
        }
        RenderApi::DrawLine({ it->X, it->Y - 0.05f, -1.0f }, { next->X, next->Y - 0.05f, -2.0f }, color);
    }
}

/*
 * ===================================================================
 * ------------------------- CarController ---------------------------
 *  ===================================================================
 */

real_t CarController::_sGasTime = 1.5;
real_t CarController::_sRotateTime = 0.2;
real_t CarController::_sJumpTime = 0.1;

void CarController::OnUpdate(DeltaTime delta)
{
    if (Input::IsKeyPressed(Key::LeftShift))
    {
        Car::_sGasPower = MAX_GAS_POWER;
    }
    else
    {
        Car::_sGasPower = NORMAL_GAS_POWER;
    }

    if (Input::IsKeyPressed(Key::A))
    {
        _RotateLeft(delta);
    }
    else if (Input::IsKeyPressed(Key::D))
    {
        _RotateRight(delta);
    }

    if (Input::IsKeyPressed(Key::W))
    {
        _Forward(delta);
    }
    else if (Input::IsKeyPressed(Key::S))
    {
        _Backward(delta);
    }

    if (Input::IsKeyPressed(Key::Space))
    {
        if (!_jumping)
        {
            _jumping = true;
            _jumpTime = 0;
            _car->Jump();
        }
    }
    else
    {
        if (_jumping)
        {
            _jumpTime += delta;
            if (_jumpTime > _sJumpTime)
            {
                _jumping = false;
                _car->StopJump();
            }
        }
    }
}

void CarController::_Forward(real_t delta)
{
    if (_forward)
    {
        _gasTime += delta;
    }
    else
    {
        // speed up decrease
        _gasTime = Math::Max(0.0, _gasTime - delta * 3);
        if (_gasTime == 0.0)
        {
            _forward = true;
        }
    }

    real_t threshold = Interpolate(0.0, 1.0, RangeToStep(0.0, _sGasTime, _gasTime), EaseInOutQuad);
    _car->Forward(threshold);
}

void CarController::_Backward(real_t delta)
{
    if (!_forward)
    {
        _gasTime += delta;
    }
    else
    {
        // speed up decrease
        _gasTime = Math::Max(0.0, _gasTime - delta * 3);
        if (_gasTime == 0.0)
        {
            _forward = false;
        }
    }

    real_t threshold = Interpolate(0.0, 1.0, RangeToStep(0.0, _sGasTime, _gasTime), EaseOutQuad);
    _car->Backward(threshold);
}

void CarController::_RotateLeft(real_t delta)
{
    if (_left)
    {
        _rotateTime += delta;
    }
    else
    {
        // speed up decrease
        _rotateTime = Math::Max(0.0, _rotateTime - delta * 3);
        if (_rotateTime == 0.0)
        {
            _left = true;
        }
    }

    real_t threshold = Interpolate(0.0, 1.0, RangeToStep(0.0, _sRotateTime, _rotateTime), EaseOutQuad);
    _car->RotateLeft(threshold);
}

void CarController::_RotateRight(real_t delta)
{
    if (!_left)
    {
        _rotateTime += delta;
    }
    else
    {
        // speed up decrease
        _rotateTime = Math::Max(0.0, _rotateTime - delta * 3);
        if (_rotateTime == 0.0)
        {
            _left = false;
        }
    }

    real_t threshold = Interpolate(0.0, 1.0, RangeToStep(0.0, _sRotateTime, _rotateTime), EaseOutQuad);
    _car->RotateRight(threshold);
}

void CarController::_Idle(real_t delta)
{
    _gasTime = Math::Max(0.0, _gasTime - delta * 2);
    _rotateTime = Math::Max(0.0, _rotateTime - delta * 2);
}
