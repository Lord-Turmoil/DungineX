#pragma once

#include <DgeX/DgeX.h>

using namespace DgeX;
using namespace DgeX::Physics;

class PlatformInterface : public Interface
{
public:
    PlatformInterface();

    void OnLoad() override;
    void OnUpdate(DeltaTime delta) override;
    void OnEvent(const Ref<Event>& event) override;
    void OnRender() override;

private:
    bool _OnKeyPressed(KeyPressedEvent& event);

    void _Init();
    void _UpdateMass();

private:
    ParticleWorld _world;
    Particle _particles[4];
    ParticleRod _rods[6];
    ParticlesPanel _panel;

    ParticleAnchoredBungee _bungees[2];
    Vector3 _bungeeAnchors[2];

    real_t _slide;

    SceneCamera _camera;
};
