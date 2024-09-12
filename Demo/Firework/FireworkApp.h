#pragma once

#include <DgeX/DgeX.h>

#include "Firework.h"

class FireworkApp : public Application
{
public:
    FireworkApp(const ApplicationSpecification& specification) : Application(specification)
    {
    }
};

class FireworkInterface : public Interface
{
public:
    FireworkInterface();
    void OnLoad() override;
    void OnUnload() override;

    void OnUpdate(DeltaTime delta) override;
    void OnEvent(const Ref<Event>& event) override;

    void OnRender() override;

private:
    bool _OnKeyPressed(KeyPressedEvent& event);

    void _CreateFirework(int type, int count, const Vector3& position);
    void _CreateFirework(int type, int count, Firework* parent);

private:
    FireworkSpecification _fireworkSpecs[9];

    ObjectPool<Firework> _fireworkPool;
    ObjectQueue<Firework> _fireworks;

    Vector3 _emitterPosition = Vector3(0, 0, 0);

    SceneCamera _camera;
};
