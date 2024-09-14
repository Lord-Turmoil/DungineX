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
}

void PlatformInterface::OnUpdate(DeltaTime delta)
{
    _world.Step(delta);

    if (Input::IsKeyPressed(Key::W))
    {
        _camera.Translate(0, 5.f);
    }
    if (Input::IsKeyPressed(Key::S))
    {
        _camera.Translate(0, -5.f);
    }
    if (Input::IsKeyPressed(Key::A))
    {
        _camera.Translate(-5.f, 0);
    }
    if (Input::IsKeyPressed(Key::D))
    {
        _camera.Translate(5.f, 0);
    }
    if (Input::IsKeyPressed(Key::Q))
    {
        _camera.Zoom(0.01f);
    }
    if (Input::IsKeyPressed(Key::E))
    {
        _camera.Zoom(-0.01f);
    }
    if (Input::IsKeyPressed(Key::R))
    {
        _camera.Rotate(0.01f);
    }
    if (Input::IsKeyPressed(Key::F))
    {
        _camera.Rotate(-0.01f);
    }
    if (Input::IsKeyPressed(Key::Right))
    {
        _slide = Math::Clamp(_slide + 0.01, 0.0, 1.0);
    }
    if (Input::IsKeyPressed(Key::Left))
    {
        _slide = Math::Clamp(_slide - 0.01, 0.0, 1.0);
    }

    _UpdateMass();
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
        auto start = rod.GetFirst()->GetPosition();
        auto end = rod.GetSecond()->GetPosition();
        RenderApi::DrawLine({ start.X, start.Y, 0 }, { end.X, end.Y, 0 }, color);
    }

    using namespace DgeX::Utils::Easing;
    auto pos = Interpolate(_particles[3].GetPosition(), _particles[2].GetPosition(), _slide, Linear);
    RenderApi::SetLineWidth(1.f);
    RenderApi::DrawCircle({ pos.X, pos.Y, 100.f }, 1.f, Color::LightBlue.ToVec4());

    RenderApi::SetLineWidth(2.f);
    RenderApi::DrawLine({ -100, _panel.GetAnchor().Y, 0 }, { 1000, _panel.GetAnchor().Y, 0 }, Color::DarkGray.ToVec4());

    RenderApi::EndScene();
}

bool PlatformInterface::_OnKeyPressed(KeyPressedEvent& event)
{
    if (event.GetKeyCode() == Key::Escape)
    {
        EventEmitter::Emit(WindowCloseEvent::Create());
        return true;
    }
    if (event.GetKeyCode() == Key::Space)
    {
        _camera.Reset();
        return true;
    }
    return false;
}

void PlatformInterface::_Init()
{
    // Create the masses and connections.
    _particles[0].SetPosition(60, 30);
    _particles[1].SetPosition(70, 30);
    _particles[2].SetPosition(100, 40);
    _particles[3].SetPosition(30, 40);

    auto gravity = Vector3(0, -9.81, 0);
    for (int i = 0; i < 4; i++)
    {
        _particles[i].SetMass(1);
        _particles[i].SetVelocity(Vector3::Zero);
        _particles[i].SetDamping(0.9f);
        _particles[i].SetAcceleration(gravity);
        _particles[i].ClearAccumulatedForce();
        _world.AddParticle(_particles + i);
    }

    _rods[0].SetFirst(_particles + 0);
    _rods[0].SetSecond(_particles + 1);
    _rods[0].SetLength(10);

    _rods[1].SetFirst(_particles + 1);
    _rods[1].SetSecond(_particles + 2);
    _rods[1].SetLength(31.6);

    _rods[2].SetFirst(_particles + 2);
    _rods[2].SetSecond(_particles + 3);
    _rods[2].SetLength(70);

    _rods[3].SetFirst(_particles + 3);
    _rods[3].SetSecond(_particles + 0);
    _rods[3].SetLength(31.6);

    _rods[4].SetFirst(_particles + 0);
    _rods[4].SetSecond(_particles + 2);
    _rods[4].SetLength(41.2);

    _rods[5].SetFirst(_particles + 1);
    _rods[5].SetSecond(_particles + 3);
    _rods[5].SetLength(41.2);

    for (int i = 0; i < 6; i++)
    {
        _world.GetContactRegistry().Add(_rods + i);
    }

    _panel.SetParticles(_particles, 4);
    _panel.SetAnchor(Vector3(0, 10, 0));
    _world.GetContactRegistry().Add(&_panel);
}

void PlatformInterface::_UpdateMass()
{
    _particles[2].SetMass(1 + 10 * _slide);
    _particles[3].SetMass(1 + 10 * (1 - _slide));
}
