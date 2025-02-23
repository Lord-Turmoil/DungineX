#include "PlatformApp.h"

PlatformInterface::PlatformInterface() : _world(100)
{
    _Init();
}

void PlatformInterface::OnLoad()
{
    _camera.SetProjection(static_cast<float>(GetWidth()), static_cast<float>(GetHeight()));
    _camera.SetScale(10.f);
    _slide = 0.5;
    _UpdateMass();
}

bool PlatformInterface::OnUpdate(DeltaTime delta)
{
    _world.StepForward(delta);

    if (Input::IsKeyPressed(Key::A))
    {
        _slide = Math::Clamp(_slide - 0.01, 0.0, 1.0);
    }
    if (Input::IsKeyPressed(Key::D))
    {
        _slide = Math::Clamp(_slide + 0.01, 0.0, 1.0);
    }

    _UpdateMass();

    return true;
}

void PlatformInterface::OnEvent(const Ref<Event>& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(DGEX_BIND_EVENT_FN(_OnKeyPressed));
}

void PlatformInterface::OnRender()
{
    RenderApi::BeginScene(_camera);

    glm::vec4 color = Color::Magenta.ToVec4();
    for (auto& particle : _particles)
    {
        auto pos = particle.GetPosition();
        RenderApi::DrawFilledCircle({ pos.X, pos.Y }, 1.f, color);
    }

    color = Color::Brown.ToVec4();
    for (auto& rod : _rods)
    {
        auto start = rod.First()->GetPosition();
        auto end = rod.Second()->GetPosition();
        RenderApi::DrawLine({ start.X, start.Y, 0 }, { end.X, end.Y, 0 }, color);
    }

    using namespace DgeX::Utils::Easing;
    auto pos = Interpolate(_particles[3].GetPosition(), _particles[2].GetPosition(), _slide, Linear);
    RenderApi::SetLineWidth(1.f);
    RenderApi::DrawCircle({ pos.X, pos.Y, 100.f }, 1.f, Color::LightBlue.ToVec4());

    RenderApi::DrawLine({ _bungeeAnchors[0].X, _bungeeAnchors[0].Y, 0 },
                        { _particles[2].GetPosition().X, _particles[2].GetPosition().Y, 0 }, Color::Green.ToVec4());
    RenderApi::DrawLine({ _bungeeAnchors[1].X, _bungeeAnchors[1].Y, 0 },
                        { _particles[3].GetPosition().X, _particles[3].GetPosition().Y, 0 }, Color::Green.ToVec4());

    RenderApi::SetLineWidth(2.f);
    RenderApi::DrawLine({ -100, _panel.Anchor.Y, 0 }, { 1000, _panel.Anchor.Y, 0 }, Color::DarkGray.ToVec4());

    RenderApi::EndScene();
}

bool PlatformInterface::_OnKeyPressed(KeyPressedEvent& event)
{
    if (event.GetKeyCode() == Key::Escape)
    {
        EventEmitter::Emit(WindowCloseEvent::Create());
    }
    if (event.GetKeyCode() == Key::Space)
    {
        _camera.Reset();
    }

    if (event.GetKeyCode() == Key::Q)
    {
        _bungees[1].RestLength = Math::Clamp(_bungees[1].RestLength - 5, 10.0, 60.0);
        DGEX_LOG_DEBUG("Left length: {}", _bungees[1].RestLength);
    }
    if (event.GetKeyCode() == Key::Z)
    {
        _bungees[1].RestLength = Math::Clamp(_bungees[1].RestLength + 5, 10.0, 60.0);
        DGEX_LOG_DEBUG("Left length: {}", _bungees[1].RestLength);
    }
    if (event.GetKeyCode() == Key::E)
    {
        _bungees[0].RestLength = Math::Clamp(_bungees[0].RestLength - 5, 10.0, 60.0);
        DGEX_LOG_DEBUG("Right length: {}", _bungees[0].RestLength);
    }
    if (event.GetKeyCode() == Key::C)
    {
        _bungees[0].RestLength = Math::Clamp(_bungees[0].RestLength + 5, 10.0, 60.0);
        DGEX_LOG_DEBUG("Right length: {}", _bungees[0].RestLength);
    }
    if (event.GetKeyCode() == Key::S)
    {
        _slide = 0.5;
    }

    return true;
}

void PlatformInterface::_Init()
{
    // Create the masses and connections.
    _particles[0].SetPosition(54, 30);
    _particles[1].SetPosition(76, 30);
    _particles[2].SetPosition(100, 46);
    _particles[3].SetPosition(30, 46);

    auto gravity = Vector3(0, -9.81, 0);
    for (int i = 0; i < 4; i++)
    {
        _particles[i].SetMass(1);
        _particles[i].SetVelocity(Vector3::Zero);
        _particles[i].SetDamping(0.9f);
        _particles[i].SetAcceleration(gravity);
        _particles[i].ClearAccumulators();
        _world.AddParticle(_particles + i);
    }

    _rods[0].First(_particles + 0);
    _rods[0].Second(_particles + 1);
    _rods[0].Length = 22;

    _rods[1].First(_particles + 1);
    _rods[1].Second(_particles + 2);
    _rods[1].Length = 28.84;

    _rods[2].First(_particles + 2);
    _rods[2].Second(_particles + 3);
    _rods[2].Length = 70;

    _rods[3].First(_particles + 3);
    _rods[3].Second(_particles + 0);
    _rods[3].Length = 28.84;

    _rods[4].First(_particles + 0);
    _rods[4].Second(_particles + 2);
    _rods[4].Length = 48.7;

    _rods[5].First(_particles + 1);
    _rods[5].Second(_particles + 3);
    _rods[5].Length = 48.7;

    _bungeeAnchors[0] = Vector3(100, 70, 0);
    _bungees[0].Anchor = &_bungeeAnchors[0];
    _bungeeAnchors[1] = Vector3(30, 70, 0);
    _bungees[1].Anchor = &_bungeeAnchors[1];
    for (int i = 0; i < 2; i++)
    {
        _bungees[i].RestLength = 40.0;
        _bungees[i].SpringConstant = 3.0;
        _forceRegistrations[i] = ParticleForceRegistration(_particles + 2 + i, _bungees + i);
        _world.GetForceRegistry().Add(_forceRegistrations + i);
    }

    for (int i = 0; i < 6; i++)
    {
        _world.GetContactRegistry().Add(_rods + i);
    }

    _panel.SetParticles(_particles, 4);
    _panel.Anchor = Vector3(0, 10, 0);
    _world.GetContactRegistry().Add(&_panel);
}

void PlatformInterface::_UpdateMass()
{
    _particles[2].SetMass(1 + 10 * _slide);
    _particles[3].SetMass(1 + 10 * (1 - _slide));
}
