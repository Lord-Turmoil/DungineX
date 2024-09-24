#pragma once

#include <DgeX/DgeX.h>
#include "Car.h"

class CarInterface : public DgeX::Interface
{
public:
    CarInterface();

    void OnLoad() override;
    void OnUnload() override;

    void OnUpdate(DgeX::DeltaTime delta) override;
    void OnEvent(const DgeX::Ref<DgeX::Event>& event) override;

    void OnRender() override;

private:
    bool _OnKeyPressed(DgeX::KeyPressedEvent& event);

    void _InitMap();

private:
    Car _car;
    CarController _controller;
    Map _map;

    DgeX::Physics::ParticleWorld _world;

    DgeX::SceneCamera _camera;
    DgeX::Physics::Vector3 _cameraCenter;
};
