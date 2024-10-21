#pragma once

DGEX_BEGIN

/**
 * @brief Base class for all cameras.
 */
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

    /**
     * @brief
     * Projection is the base matrix for the camera, it defines the area that
     * the camera can see. If used alone, will be a rectangle area with the
     * top-left corner at (0, 0) and the bottom-right corner at (1, 1).
     *
     * @return the projection
     */
    const glm::mat4& GetProjection() const
    {
        return _projection;
    }

    /**
     * @brief
     * The view is the matrix that represents the camera position and rotation.
     * By default, it is the identity matrix, i.e., the camera is at the origin.
     *
     * @return the view
     */
    virtual const glm::mat4& GetView() const
    {
        static glm::mat4 view(1.0f);
        return view;
    }

    /**
     * @brief
     * View projection is the combination of the view and projection matrices.
     * With this matrix, you can transform the world coordinates to the screen.
     *
     * @return the view projection
     */
    virtual const glm::mat4& GetViewProjection() const
    {
        return _projection;
    }

protected:
    glm::mat4 _projection;
};

DGEX_END
