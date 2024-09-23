#include "Car.h"

using namespace DgeX;
using namespace DgeX::Physics;
using namespace DgeX::Utils::Easing;

static real_t Distance(const Particle& p0, const Particle& p1)
{
    return (p0.GetPosition() - p1.GetPosition()).Magnitude();
}

// clang-format off
Vector3 Car::_sCenterOffset = Vector3(2.4, 0.8);

Vector3 Car::_sBodyOffset[4] = {
    Vector3(0.0, 1.6) - _sCenterOffset,
    Vector3(4.8, 1.6) - _sCenterOffset,
    Vector3(3.0, 1.0) - _sCenterOffset,
    Vector3(1.8, 1.0) - _sCenterOffset
};

Vector3 Car::_sWheelOffset[2] = {
    Vector3(0.6, 0.55) - _sCenterOffset,
    Vector3(4.2, 0.55) - _sCenterOffset
};

real_t Car::_sWheelRadius = 0.55;
real_t Car::_sSpringConstant = 150.0;
real_t Car::_sSpringRestLength = (_sBodyOffset[0] - _sWheelOffset[0]).Magnitude() + 0.2;

real_t Car::_sPower = 800.0;
real_t Car::_sRotatePower = 50.0;

// clang-format on

Car::Car()
{
    _body = new Particle[4];
    _wheel = new Particle[2];
    _frame = new ParticleRod[8];
    _spring = new ParticleSpring[4];
    _springLimit = new ParticleHalfRod[2];

    _Init(Vector3::Zero);
}

Car::~Car()
{
    delete[] _body;
    delete[] _wheel;
    delete[] _frame;
    delete[] _spring;
    delete[] _springLimit;
}

void Car::Forward(real_t threshold)
{
    _wheelForce[0].SetForce(_wheelNormal[0] * _sPower * threshold);
    _wheelForce[1].SetForce(_wheelNormal[1] * _sPower * threshold);

    if (!_wheelNormal[0].IsZero())
    {
        Vector3 correctedRotation = Vector3::UnitY * _sRotatePower * threshold * threshold * 0.2;
        _rotateForce[1].SetForce(_rotateForce[1].GetForce() + correctedRotation);
    }

    _desiredRotation[0] = _desiredRotation[1] = _sPower * threshold * 0.1;
}

void Car::Backward(real_t threshold)
{
    _wheelForce[0].SetForce(_wheelNormal[0] * _sPower * -threshold);
    _wheelForce[1].SetForce(_wheelNormal[1] * _sPower * -threshold);

    if (!_wheelNormal[1].IsZero())
    {
        Vector3 correctedRotation = Vector3::UnitY * _sRotatePower * threshold * threshold * 0.2;
        _rotateForce[0].SetForce(_rotateForce[0].GetForce() + correctedRotation);
    }

    _desiredRotation[0] = _desiredRotation[1] = -_sPower * threshold * 0.1;
}

void Car::RotateLeft(real_t threshold)
{
    _rotateForce[1].SetForce(Vector3::UnitY * _sRotatePower * threshold);
}

void Car::RotateRight(real_t threshold)
{
    _rotateForce[0].SetForce(Vector3::UnitY * _sRotatePower * threshold);
}

void Car::Jump() const
{
    for (int i = 0; i < 4; i++)
    {
        _spring[i].SetRestLength(_sSpringRestLength * 1.2);
        _spring[i].SetSpringConstant(_sSpringConstant * 20);
    }
}

void Car::StopJump() const
{
    for (int i = 0; i < 4; i++)
    {
        _spring[i].SetRestLength(_sSpringRestLength);
        _spring[i].SetSpringConstant(_sSpringConstant);
    }
}

void Car::ResetState()
{
    _wheelForce[0].SetForce(Vector3::Zero);
    _wheelForce[1].SetForce(Vector3::Zero);

    _rotateForce[0].SetForce(Vector3::Zero);
    _rotateForce[1].SetForce(Vector3::Zero);
}

void Car::Reset(const Vector3& center)
{
    _Init(center);
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
    for (int i = 0; i < 4; i++)
    {
        world.AddParticle(&_body[i]);
    }

    auto& forceRegistry = world.GetForceRegistry();
    forceRegistry.Add(&_body[2], &_wheelForce[0]);
    forceRegistry.Add(&_body[3], &_wheelForce[1]);
    forceRegistry.Add(&_body[0], &_rotateForce[0]);
    forceRegistry.Add(&_body[1], &_rotateForce[1]);
    for (int i = 0; i < 4; i++)
    {
        forceRegistry.Add(&_body[i], &_bodyDrag);
    }
    for (int i = 0; i < 2; i++)
    {
        forceRegistry.Add(&_wheel[i], &_spring[i]);
        forceRegistry.Add(&_wheel[i], &_wheelDrag);
    }
    for (int i = 0; i < 2; i++)
    {
        forceRegistry.Add(&_body[i], &_spring[2 + i]);
    }

    auto& contactRegistry = world.GetContactRegistry();
    for (int i = 0; i < 8; i++)
    {
        contactRegistry.Add(&_frame[i]);
    }
    contactRegistry.Add(&_springLimit[0]);
    contactRegistry.Add(&_springLimit[1]);

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
    for (int i = 0; i < 4; i++)
    {
        world.RemoveParticle(&_body[i]);
    }

    auto& forceRegistry = world.GetForceRegistry();
    forceRegistry.Remove(&_body[2], &_wheelForce[0]);
    forceRegistry.Remove(&_body[3], &_wheelForce[1]);
    forceRegistry.Remove(&_body[0], &_rotateForce[0]);
    forceRegistry.Remove(&_body[1], &_rotateForce[1]);
    for (int i = 0; i < 4; i++)
    {
        forceRegistry.Remove(&_body[i], &_bodyDrag);
    }
    for (int i = 0; i < 2; i++)
    {
        forceRegistry.Remove(&_wheel[i], &_spring[i]);
        forceRegistry.Remove(&_wheel[i], &_wheelDrag);
    }
    for (int i = 0; i < 2; i++)
    {
        forceRegistry.Remove(&_body[i], &_spring[2 + i]);
    }

    auto& contactRegistry = world.GetContactRegistry();
    for (int i = 0; i < 8; i++)
    {
        contactRegistry.Remove(&_frame[i]);
    }
    contactRegistry.Remove(&_springLimit[0]);
    contactRegistry.Remove(&_springLimit[1]);

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
                _actualRotation[i] *= 0.99;
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
}

void Car::OnRender() const
{
    auto wheelColor = Color::Brown.ToVec4();
    auto wheelRodColor = Color::FromUInt32(0xFF171310).ToVec4();
    auto frameColor = Color::FromUInt32(0xFF686D4D).ToVec4();
    auto pointColor = Color::DarkGray.ToVec4();
    auto springColor = Color::LightGray.ToVec4();

    for (int i = 0; i < 4; i++)
    {
        RenderApi::DrawFilledCircle(_body[i].GetPosition().ToGlmVec3(), 0.1f, pointColor);
    }

    RenderApi::SetLineWidth(4.f);
    for (int i = 0; i < 8; i++)
    {
        auto start = _frame[i].GetFirst()->GetPosition().ToGlmVec3();
        auto end = _frame[i].GetSecond()->GetPosition().ToGlmVec3();
        RenderApi::DrawLine(start, end, frameColor);
    }

    RenderApi::SetLineWidth(5.f);
    for (int i = 0; i < 2; i++)
    {
        auto start = _body[i].GetPosition().ToGlmVec3();
        auto end = _wheel[i].GetPosition().ToGlmVec3();
        RenderApi::DrawLine(start, end, springColor);
    }

    RenderApi::SetLineWidth(10.0f);
    for (int i = 0; i < 2; i++)
    {
        auto pos = _wheel[i].GetPosition().ToGlmVec3();
        glm::vec3 start = { -Math::Cos(_wheelRadian[i]), Math::Sin(_wheelRadian[i]), 0.f };
        glm::vec3 end = { Math::Cos(_wheelRadian[i]), -Math::Sin(_wheelRadian[i]), 0.f };
        start = start * static_cast<float>(_sWheelRadius) + pos;
        end = end * static_cast<float>(_sWheelRadius) + pos;
        RenderApi::DrawLine(start, end, wheelRodColor);

        RenderApi::DrawFilledCircle(pos, 0.1f, pointColor);
        RenderApi::DrawCircle(pos, static_cast<float>(_sWheelRadius), wheelColor);
    }
}

void Car::_Init(const Vector3& center)
{
    Vector3 gravity = Vector3(0, -10.0, 0);

    for (int i = 0; i < 2; i++)
    {
        _wheel[i].SetPosition(center + _sWheelOffset[i]);
        _wheel[i].SetMass(1);
        _wheel[i].SetDamping(0.4);
        _wheel[i].SetAcceleration(gravity);
    }

    for (int i = 0; i < 4; i++)
    {
        _body[i].SetPosition(center + _sBodyOffset[i]);
        _body[i].SetMass(2);
        _body[i].SetDamping(0.8);
        _body[i].SetAcceleration(gravity);
    }

    // outer frame
    for (int i = 0; i < 4; i++)
    {
        _frame[i] = ParticleRod(&_body[i], &_body[(i + 1) % 4], Distance(_body[i], _body[(i + 1) % 4]));
    }

    // cross frame
    _frame[4] = ParticleRod(&_body[0], &_body[2], Distance(_body[0], _body[2]));
    _frame[5] = ParticleRod(&_body[1], &_body[3], Distance(_body[1], _body[3]));

    // wheel rods
    _frame[6] = ParticleRod(&_body[3], &_wheel[0], Distance(_body[3], _wheel[0]));
    _frame[7] = ParticleRod(&_body[2], &_wheel[1], Distance(_body[2], _wheel[1]));

    // spring
    for (int i = 0; i < 2; i++)
    {
        _spring[i] = ParticleSpring(&_body[i], _sSpringConstant, _sSpringRestLength);
        _springLimit[i] = ParticleHalfRod(&_body[i], &_wheel[i], 0.8, 0.8);
    }
    for (int i = 0; i < 2; i++)
    {
        _spring[2 + i] = ParticleSpring(&_wheel[i], _sSpringConstant, _sSpringRestLength);
    }

    _bodyDrag = ParticleDrag(0.6, 0.25);
    _wheelDrag = ParticleDrag(0.2, 0.1);

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
 * ------------------------------- Map -------------------------------
 * ===================================================================
 */

uint32_t Map::AddContact(ParticleContact* contact, uint32_t limit) const
{
    _car->_wheelNormal[0] = _car->_wheelNormal[1] = Vector3::Zero;

    uint32_t used = 0;
    for (int i = 0; (i < 2) && (used < limit); i++)
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

        for (it = left; (it != right) && (used < limit); ++it)
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
                    contact->SetFirst(particle);
                    contact->SetSecond(nullptr);
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
                    contact->SetFirst(particle);
                    contact->SetSecond(nullptr);
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
                real_t distanceSquare = relativePosition.MagnitudeSquare() - projected * projected;
                if (distanceSquare < Car::_sWheelRadius * Car::_sWheelRadius)
                {
                    Vector3 contactPoint = start + direction * (projected / direction.Magnitude());

                    contact->SetFirst(particle);
                    contact->SetSecond(nullptr);
                    contact->ContactNormal = (particle->GetPosition() - contactPoint).Normalized();
                    contact->Penetration = Car::_sWheelRadius - Math::Sqrt(distanceSquare);
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

/*
 * ===================================================================
 * ------------------------- CarController ---------------------------
 *  ===================================================================
 */

real_t CarController::_sGasTime = 0.8;
real_t CarController::_sRotateTime = 0.2;
real_t CarController::_sJumpTime = 0.001; // one frame

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
        RenderApi::DrawLine({ it->X, it->Y - 0.1f, -1.0f }, { next->X, next->Y - 0.1f, -2.0f }, color);
    }
}

void CarController::OnUpdate(DeltaTime delta)
{
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
        else
        {
            // prevent constant jump
            _car->StopJump();
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
