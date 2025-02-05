#pragma once

#include "DgeX/Scene/Camera/Camera.h"

DGEX_BEGIN

// clang-format off
/**
 * @brief
 * Although DungineX only provide 2D features, it still has a perspective camera
 * to support more vivid 2D effects.
 *
 * @note
 * This camera is not complete and requires further development.
 */
class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float radian, float aspectRatio, float nearClip, float farClip);
    PerspectiveCamera(float radian, float width, float height, float nearClip, float farClip);

    void SetFov(float radian) { _fov = radian; _RecalculateViewProjection(); }
    float GetFov() const { return _fov; }

    void SetAspectRatio(float aspectRatio) { _aspectRatio = aspectRatio; _RecalculateViewProjection(); }
    void SetAspectRatio(float width, float height) { SetAspectRatio(width / height); }
    float GetAspectRatio() const { return _aspectRatio; }

    void SetNearClip(float nearClip) { _nearClip = nearClip; _RecalculateViewProjection(); }
    float GetNearClip() const { return _nearClip; }
    void SetFarClip(float farClip) { _farClip = farClip; _RecalculateViewProjection(); }
    float GetFarClip() const { return _farClip; }

private:
    void _RecalculateViewProjection();

private:
    glm::mat4 _view;
    glm::mat4 _viewProjection;

    float _fov = glm::radians(45.0f);
    float _aspectRatio;
    float _nearClip = 0.01f;
    float _farClip = 1000.f;
};

// clang-format on

DGEX_END
