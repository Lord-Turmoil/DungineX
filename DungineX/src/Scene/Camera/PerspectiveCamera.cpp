#include "DgeX/Scene/Camera/PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

DGEX_BEGIN

PerspectiveCamera::PerspectiveCamera(float radian, float aspectRatio, float nearClip, float farClip)
    : _fov(radian), _aspectRatio(aspectRatio), _nearClip(nearClip), _farClip(farClip)
{
    _RecalculateViewProjection();
}

PerspectiveCamera::PerspectiveCamera(float radian, float width, float height, float nearClip, float farClip)
    : _fov(radian), _aspectRatio(width / height), _nearClip(nearClip), _farClip(farClip)
{
    _RecalculateViewProjection();
}

void PerspectiveCamera::_RecalculateViewProjection()
{
    _projection = glm::perspective(_fov, _aspectRatio, _nearClip, _farClip);
}

DGEX_END
