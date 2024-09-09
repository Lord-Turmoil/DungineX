#pragma once

#include "DgeX/Renderer/Camera/Camera.h"

DGEX_BEGIN

class InterfaceCamera : public Camera
{
public:
    InterfaceCamera(float width, float height);

    void OnResize(float width, float height);
};

DGEX_END
