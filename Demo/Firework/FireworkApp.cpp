#include "FireworkApp.h"

Interface* DgeX::GetInterface(const std::string& name)
{
    static Interface* interface = new FireworkInterface();

    return interface;
}

FireworkInterface::FireworkInterface() : _fireworkPool(1000)
{
    _fireworkSpecs[0].SetParameters(1,                   // type
                                    0.5f, 1.4f,          // age range
                                    Vector3(-5, 25, -5), // min velocity
                                    Vector3(5, 28, 5),   // max velocity
                                    0.1                  // damping
    );
    _fireworkSpecs[0].AddPayload(3, 5);
    _fireworkSpecs[0].AddPayload(5, 5);

    _fireworkSpecs[1].SetParameters(2,                   // type
                                    0.5f, 1.0f,          // age range
                                    Vector3(-5, 10, -5), // min velocity
                                    Vector3(5, 20, 5),   // max velocity
                                    0.8                  // damping
    );
    _fireworkSpecs[1].AddPayload(4, 2);

    _fireworkSpecs[2].SetParameters(3,                   // type
                                    0.5f, 1.5f,          // age range
                                    Vector3(-5, -5, -5), // min velocity
                                    Vector3(5, 5, 5),    // max velocity
                                    0.1                  // damping
    );

    _fireworkSpecs[3].SetParameters(4,                   // type
                                    0.25f, 0.5f,         // age range
                                    Vector3(-20, 5, -5), // min velocity
                                    Vector3(20, 5, 5),   // max velocity
                                    0.2                  // damping
    );

    _fireworkSpecs[4].SetParameters(5,                   // type
                                    0.5f, 1.0f,          // age range
                                    Vector3(-20, 2, -5), // min velocity
                                    Vector3(20, 18, 5),  // max velocity
                                    0.01                 // damping
    );
    _fireworkSpecs[4].AddPayload(3, 5);

    _fireworkSpecs[5].SetParameters(6,                  // type
                                    3, 5,               // age range
                                    Vector3(-5, 5, -5), // min velocity
                                    Vector3(5, 10, 5),  // max velocity
                                    0.95                // damping
    );

    _fireworkSpecs[6].SetParameters(7,                   // type
                                    4, 5,                // age range
                                    Vector3(-5, 50, -5), // min velocity
                                    Vector3(5, 60, 5),   // max velocity
                                    0.01                 // damping
    );
    _fireworkSpecs[6].AddPayload(8, 10);

    _fireworkSpecs[7].SetParameters(8,                   // type
                                    0.25f, 0.5f,         // age range
                                    Vector3(-1, -1, -1), // min velocity
                                    Vector3(1, 1, 1),    // max velocity
                                    0.01                 // damping
    );

    _fireworkSpecs[8].SetParameters(9,                    // type
                                    3, 5,                 // age range
                                    Vector3(-15, 10, -5), // min velocity
                                    Vector3(15, 15, 5),   // max velocity
                                    0.95                  // damping
    );
}

void FireworkInterface::OnLoad()
{
    _camera.SetProjection(static_cast<float>(GetWidth()), static_cast<float>(GetHeight()));
    _camera.SetScale(50.f);

    auto [x, y] = Input::GetInverseMousePosition();
    _emitterPosition.Set(_camera.ScreenToWorld(static_cast<float>(x), static_cast<float>(y)));
}

void FireworkInterface::OnUnload()
{
    for (auto it : _fireworks)
    {
        it->Dispose();
    }
    _fireworks.Clear();
}

void FireworkInterface::OnUpdate(DeltaTime delta)
{
    real_t step = delta.Seconds();
    std::vector<Firework*> toRemove;

    _fireworks.Lock();
    for (auto firework : _fireworks)
    {
        if (firework->Update(step))
        {
            for (auto [Type, Count] : _fireworkSpecs[firework->GetType() - 1].Payloads)
            {
                _CreateFirework(Type, Count, firework);
            }
            firework->Dispose();
            _fireworks.Remove(firework);
        }
    }
    _fireworks.Unlock();

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

    auto [x, y] = Input::GetInverseMousePosition();
    _emitterPosition.Set(_camera.ScreenToWorld(static_cast<float>(x), static_cast<float>(y)));
}

void FireworkInterface::OnEvent(const Ref<Event>& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(DGEX_BIND_EVENT_FN(FireworkInterface::_OnKeyPressed));
}

void FireworkInterface::OnRender()
{
    RenderApi::BeginScene(_camera);
    for (auto it : _fireworks)
    {
        glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
        switch (it->GetType())
        {
        case 1:
            color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case 2:
            color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
            break;
        case 3:
            color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
            break;
        case 4:
            color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case 5:
            color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 6:
            color = glm::vec4(0.4f, 0.4f, 1.0f, 1.0f);
            break;
        case 7:
            color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
            break;
        case 8:
            color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case 9:
            color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
            break;
        default:
            break;
        }

        Vector3 position = it->GetPosition();
        float size = static_cast<float>(Utils::Easing::Interpolate(
            0.1, 1.0, Utils::Easing::RangeToStep(-5.0, 20.0, position.Z), Utils::Easing::Linear));
        RenderApi::DrawQuad({ position.X, position.Y }, { size, size }, color);
    }
    RenderApi::EndScene();
}

bool FireworkInterface::_OnKeyPressed(KeyPressedEvent& event)
{
    switch (event.GetKeyCode())
    {
    case Key::D1:
        _CreateFirework(1, 1, _emitterPosition);
        break;
    case Key::D2:
        _CreateFirework(2, 1, _emitterPosition);
        break;
    case Key::D3:
        _CreateFirework(3, 1, _emitterPosition);
        break;
    case Key::D4:
        _CreateFirework(4, 1, _emitterPosition);
        break;
    case Key::D5:
        _CreateFirework(5, 1, _emitterPosition);
        break;
    case Key::D6:
        _CreateFirework(6, 1, _emitterPosition);
        break;
    case Key::D7:
        _CreateFirework(7, 1, _emitterPosition);
        break;
    case Key::D8:
        _CreateFirework(8, 1, _emitterPosition);
        break;
    case Key::D9:
        _CreateFirework(9, 1, _emitterPosition);
        break;
    case Key::Space:
        // reset
        _camera.Reset();
        break;
    default:
        break;
    }
    return true;
}

void FireworkInterface::_CreateFirework(int type, int count, const Vector3& position)
{
    FireworkSpecification* spec = &_fireworkSpecs[type - 1];
    for (int i = 0; i < count; i++)
    {
        Firework* firework = _fireworkPool.TryAcquire();
        if (!firework)
        {
            break;
        }
        spec->Create(firework, position);
        _fireworks.Add(firework);
    }
}

void FireworkInterface::_CreateFirework(int type, int count, Firework* parent)
{
    FireworkSpecification* spec = &_fireworkSpecs[type - 1];
    for (int i = 0; i < count; i++)
    {
        Firework* firework = _fireworkPool.TryAcquire();
        if (!firework)
        {
            break;
        }
        spec->Create(firework, parent);
        _fireworks.Add(firework);
    }
}
