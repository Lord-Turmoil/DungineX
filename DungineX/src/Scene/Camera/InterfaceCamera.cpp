#include "DgeX/Scene/Camera/InterfaceCamera.h"

#include <glm/ext/matrix_clip_space.hpp>

DGEX_BEGIN

InterfaceCamera::InterfaceCamera(float width, float height)
{
    OnResize(width, height);
}

void InterfaceCamera::OnResize(float width, float height)
{
    _projection = glm::ortho(0.f, width, height, 0.f, 1000.f, -1000.f);
}

DGEX_END
