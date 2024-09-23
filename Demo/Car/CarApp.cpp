#include "CarApp.h"

using namespace DgeX;
using namespace DgeX::Physics;

CarInterface::CarInterface() : Interface("Car"), _world(128)
{
}

void CarInterface::OnLoad()
{
    RenderApi::SetClearColor(Color::White.ToVec4());

    _car.Reset(Vector3(0, 10));
    _car.Register(_world);

    _map.PushBack(Vector3(-1000.0, 5.0, 0.0));
    _map.PushBack(Vector3(1000.0, 5.0, 0.0));
    _map.SetCar(&_car);
    _world.GetContactRegistry().Add(&_map);

    _controller.SetCar(&_car);

    _camera.SetProjection(static_cast<float>(GetWidth()), static_cast<float>(GetHeight()));
    _camera.SetWorldHeight(12);
    _camera.CenterWorldPoint(0, 8);
}

void CarInterface::OnUnload()
{
    _car.Unregister(_world);
}

void CarInterface::OnUpdate(DeltaTime delta)
{
    _world.Step(delta);
    _car.ResetState();
    _controller.OnUpdate(delta);
    _car.OnUpdate(delta);
}

void CarInterface::OnEvent(const Ref<Event>& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(DGEX_BIND_EVENT_FN(CarInterface::_OnKeyPressed));
}

void CarInterface::OnRender()
{
    RenderApi::BeginScene(_camera);

    RenderApi::DrawFilledCircle({ 0, 0 }, 0.5f, Color::Red.ToVec4());

    _car.OnRender();
    _map.OnRender();

    RenderApi::EndScene();
}

bool CarInterface::_OnKeyPressed(KeyPressedEvent& event)
{
    if (event.GetKeyCode() == Key::Escape)
    {
        EventEmitter::Emit(WindowCloseEvent::Create());
        return true;
    }
    return false;
}
