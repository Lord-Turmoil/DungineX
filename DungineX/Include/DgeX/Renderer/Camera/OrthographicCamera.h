#pragma once

#include "DgeX/Renderer/Camera/Camera.h"

DGEX_BEGIN

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(float x, float y, float width, float height);
    OrthographicCamera(glm::vec2 pos, glm::vec2 size);

    void SetProjection(float x, float y, float width, float height);
    void SetProjection(glm::vec2 pos, glm::vec2 size);

    const glm::vec3& GetPosition() const
    {
        return _position;
    }

    void SetPosition(const glm::vec3& position)
    {
        _position = position;
        _RecalculateViewProjection();
    }

    float GetRotation() const
    {
        return _rotation;
    }

    void SetRotation(float rotation)
    {
        _rotation = rotation;
        _RecalculateViewProjection();
    }

    const glm::mat4& GetView() const override
    {
        return _view;
    }

    const glm::mat4& GetViewProjection() const override
    {
        return _viewProjection;
    }

private:
    void _RecalculateViewProjection();

private:
    glm::mat4 _view;
    glm::mat4 _viewProjection;

    glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
    float _rotation = 0.0f;
};

DGEX_END
