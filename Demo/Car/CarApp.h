#pragma once

#include <DgeX/DgeX.h>

#include "Car.h"

class CarLayer : public DgeX::Layer
{
    void OnRender() override;
};

class CarInterface : public DgeX::Interface
{
public:
    CarInterface();

    void OnLoad() override;
    void OnUnload() override;

    bool OnUpdate(DgeX::DeltaTime delta) override;
    void OnEvent(const DgeX::Ref<DgeX::Event>& event) override;

    void OnRender() override;

private:
    bool _OnKeyPressed(DgeX::KeyPressedEvent& event);
    void _OnReload();
    void _InitMap();

private:
    Car _car;
    CarController _carController;
    Map _map;
    DustController _dustController;

    DgeX::Physics::ParticleWorld _world;

    DgeX::SceneCamera _camera;
    DgeX::Physics::Vector3 _cameraCenter;

    DgeX::Ref<CarLayer> _carLayer;
};
