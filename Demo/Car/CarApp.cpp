#include "CarApp.h"

using namespace DgeX;
using namespace DgeX::Physics;

void CarLayer::OnRender()
{
    static std::string help = R"(How to play?
Use 'W' and 'S' to accelerate and brake.
Use 'A' and 'D' to steer left and right.
Press 'R' to reset the car.
Press 'F' to reset the game.
)";
    RenderApi::SetTextStyle(32.f);
    RenderApi::DrawString(help, { 5.0f, 5.0f }, Color::Black.ToVec4());
}

CarInterface::CarInterface() : Interface("Car"), _dustController(512), _world(128), _camera()
{
    _carLayer = CreateRef<CarLayer>();
}

void CarInterface::OnLoad()
{
    RenderApi::SetClearColor(Color::White.ToVec4());

    _car.Reset(Vector3(0, 5));
    _car.Register(_world);

    _InitMap();
    _map.SetCar(&_car);
    _world.GetContactRegistry().Add(&_map);
    _world.SetSimulationRate(1000);
    _world.EnableAdaptive(Application::GetInstance()->GetRefreshRate());

    _carController.SetCar(&_car);
    _dustController.SetCar(&_car);
    _dustController.SetWorld(&_world);

    _camera.SetProjection(static_cast<float>(GetWidth()), static_cast<float>(GetHeight()));
    _camera.SetWorldHeight(12);

    _cameraCenter = _car.GetCenter();
    _camera.CenterWorldPoint(_cameraCenter);

    PushLayer(_carLayer.get());
}

void CarInterface::OnUnload()
{
    _car.Unregister(_world);
}

void CarInterface::OnUpdate(DeltaTime delta)
{
    static timestamp_t elapsed = 0;

    _world.Step(delta);

    _car.ResetState();
    _carController.OnUpdate(delta);
    _car.OnUpdate(delta);
    _dustController.OnUpdate(delta);

    using namespace DgeX::Utils::Easing;

    auto newCenter = _car.GetCenter();
    newCenter.Y += 2.0;
    _cameraCenter = Interpolate(_cameraCenter, newCenter, 0.05, Linear);
    _camera.CenterWorldPoint(_cameraCenter);

    real_t distance = (newCenter - _cameraCenter).Magnitude();
    double zoom = Interpolate(1.2, 0.5, RangeToStep(0.0, 5.0, distance), Linear);
    _camera.SetZoom(static_cast<float>(zoom));

    elapsed += delta;
    if (elapsed > 5.0)
    {
        DGEX_LOG_INFO("Speed: {0}", _car.GetSpeed());
        elapsed = 0.0;
    }
}

void CarInterface::OnEvent(const Ref<Event>& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(DGEX_BIND_EVENT_FN(CarInterface::_OnKeyPressed));
}

void CarInterface::OnRender()
{
    RenderApi::BeginScene(_camera);

    _car.OnRender();
    _map.OnRender();
    _dustController.OnRender();

    RenderApi::EndScene();
}

bool CarInterface::_OnKeyPressed(KeyPressedEvent& event)
{
    if (event.GetKeyCode() == Key::Escape)
    {
        EventEmitter::Emit(WindowCloseEvent::Create());
        return true;
    }
    if (event.GetKeyCode() == Key::R && !event.IsRepeat())
    {
        Vector3 center = _car.GetCenter();
        center.Y += 3.0f;
        _car.Reset(center);
    }
    if (event.GetKeyCode() == Key::F && !event.IsRepeat())
    {
        // reset game
        OnLoad();
        return true;
    }

    return false;
}

void CarInterface::_InitMap()
{
    using namespace DgeX::Utils::Random;

    real_t f = 0.0;  // f
    real_t f1 = 0.0; // f'
    real_t f2 = 0.0; // f''
    real_t delta = 0.5;
    const real_t amplitude = 0.05;

    real_t x = -20.0;
    real_t nextChange = 50.0;
    real_t end = 1000.0;

    _map.Clear();

    // A wall at start.
    _map.PushBack(Vector3(x, 20.0, 0.0));

    while (x < end)
    {
        f1 += f2;
        f += f1;
        _map.PushBack(Vector3(x, f, 0.0));
        x += delta;
        if (x > nextChange || Math::Abs(f1) > 0.25)
        {
            nextChange += RandomNumber(5.0, 20.0);
            if (f1 > 0.25)
            {
                f2 = RandomNumber(-amplitude, 0.0);
            }
            else if (f1 < -0.25)
            {
                f2 = RandomNumber(0.0, amplitude);
            }
            else
            {
                f2 = RandomNumber(-amplitude, amplitude);
            }
        }
    }

    // Another wall at end.
    _map.PushBack(Vector3(x, f + 20, 0.0));
}
