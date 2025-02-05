#include "DgeX/Scene/Camera/SceneCamera.h"
#include "DgeX/Utils/Math.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

DGEX_BEGIN

SceneCamera::SceneCamera(float width, float height) : _width(width), _height(height), _translation(0.0f)
{
    _RecalculateViewProjection();
}

SceneCamera::SceneCamera(float width, float height, float scale)
    : _width(width), _height(height), _scale(scale), _translation(0.0f)
{
    _RecalculateViewProjection();
}

void SceneCamera::SetProjection(float width, float height)
{
    _width = width;
    _height = height;
    _RecalculateViewProjection();
}

void SceneCamera::SetScale(float scale)
{
    _scale = Math::Clamp(scale, 0.001f, 100.0f);
    _RecalculateViewProjection();
}

void SceneCamera::SetZoom(float zoom)
{
    _zoom = Math::Clamp(zoom, 0.01f, 100.0f);
    _RecalculateViewProjection();
}

void SceneCamera::CenterWorldPoint(float x, float y)
{
    x = (x - GetWorldWidth() * 0.5f) * _scale;
    y = (y - GetWorldHeight() * 0.5f) * _scale;
    SetTranslation(x, y);
}

Physics::Vector3 SceneCamera::ScreenToWorld(float x, float y) const
{
    auto pos = _view * glm::vec4(x, y, 0.0f, 1.0f);
    return { pos.x, pos.y, 0.0f };
}

void SceneCamera::Reset()
{
    _translation = glm::vec2(0.0f);
    _rotation = 0.0f;
    _zoom = 1.0f;
    _RecalculateViewProjection();
}

void SceneCamera::_RecalculateViewProjection()
{
    _projection = glm::ortho(0.f, _width, 0.f, _height, -1000.f, 1000.f);

    _view = scale(glm::mat4(1.0f), glm::vec3(1.f / _scale, 1.f / _scale, 1.0f));
    if ((_translation.x != 0.0f) || (_translation.y != 0.0f))
    {
        _view *= translate(glm::mat4(1.0f), glm::vec3(_translation.x, _translation.y, 0.0f));
    }
    if (_zoom != 1.0f)
    {
        float zoom = 1.0f / _zoom;
        float correctionWidth = _width * (zoom - 1) * 0.5f;
        float correctionHeight = _height * (zoom - 1) * 0.5f;
        _view *= translate(glm::mat4(1.0f), glm::vec3(-correctionWidth, -correctionHeight, 0.0f)) *
                 scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, 1.0f));
    }
    if (_rotation != 0.0f)
    {
        _view *= translate(glm::mat4(1.0f), glm::vec3(_width * 0.5f, _height * 0.5f, 0.0f)) *
                 rotate(glm::mat4(1.0f), _rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
                 translate(glm::mat4(1.0f), glm::vec3(-_width * 0.5f, -_height * 0.5f, 0.0f));
    }

    _viewProjection = _projection * inverse(_view);
}

DGEX_END
