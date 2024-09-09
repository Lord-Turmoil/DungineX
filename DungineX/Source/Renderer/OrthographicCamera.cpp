#include "DgeX/Renderer/Camera/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

DGEX_BEGIN

OrthographicCamera::OrthographicCamera(float x, float y, float width, float height)
    : Camera(glm::ortho(x, x + width, y + height, y)), _view(1.0f)
{
    _viewProjection = _projection * _view;
}

OrthographicCamera::OrthographicCamera(glm::vec2 pos, glm::vec2 size)
    : Camera(glm::ortho(pos.x, pos.x + size.x, pos.y + size.y, pos.y)), _view(1.0f)
{
    _viewProjection = _projection * _view;
}

void OrthographicCamera::SetProjection(float x, float y, float width, float height)
{
    _projection = glm::ortho(x, x + width, y + height, y);
    _viewProjection = _projection * _view;
}

void OrthographicCamera::SetProjection(glm::vec2 pos, glm::vec2 size)
{
    SetProjection(pos.x, pos.y, size.x, size.y);
}

void OrthographicCamera::_RecalculateViewProjection()
{
    glm::mat4 transform =
        translate(glm::mat4(1.0f), _position) * rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1));

    _view = inverse(transform);
    _viewProjection = _projection * _view;
}

DGEX_END
