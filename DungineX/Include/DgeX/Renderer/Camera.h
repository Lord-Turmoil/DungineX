#pragma once

#include "DgeX/dgexpch.h"

#include <glm/glm.hpp>

DGEX_BEGIN

class Camera
{
public:
    Camera(const glm::mat4& projection = glm::mat4(1.0f)) : _projection(projection)
    {
    }

    Camera(const Camera& other) = default;
    Camera(Camera&& other) noexcept = default;
    Camera& operator=(const Camera& other) = default;
    Camera& operator=(Camera&& other) noexcept = default;

    virtual ~Camera() = default;

    const glm::mat4& GetProjection() const
    {
        return _projection;
    }

protected:
    glm::mat4 _projection;
};

DGEX_END
