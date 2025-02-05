#pragma once

#include "DgeX/Physics/Physics.h"
#include "DgeX/Scene/Camera/Camera.h"

#include <glm/glm.hpp>

DGEX_BEGIN

/**
 * @brief
 * Scene camera is used to display the game world. It is a 2D camera that can
 * be rotated and zoomed in/out.
 *
 * @note
 * The position of camera displayed on the screen is the top-left corner, while
 * the origin of the world it captures is the center of the camera.
 */
// clang-format off
class SceneCamera : public Camera
{
public:
    SceneCamera() = default;
    SceneCamera(float width, float height);
    SceneCamera(float width, float height, float scale);

    const glm::mat4& GetView() const override { return _view; }
    const glm::mat4& GetViewProjection() const override { return _viewProjection; }

    /**
     * @brief Projection area is how much of the screen the camera will take up.
     * @param width Width of the pixels on screen.
     * @param height Height of the pixels on screen.
     */
    void SetProjection(float width, float height);
    float GetWidth() const { return _width; }
    float GetHeight() const { return _height; }

    /**
     * @brief
     * Scale determines the size of the world that the camera captures.
     * By default, the scale is 1.0f, i.e. the world size it captures is
     * the same as the projection size.
     *
     * @note
     * Scale's origin is (0, 0) in the world, > 1 will show less world, < 1 will
     * show more world.
     *
     * @param scale The scale of the camera world.
     */
    void SetScale(float scale);
    void SetWorldHeight(float height) { SetScale(_height / height); }
    void SetWorldWidth(float width) { SetScale(_width / width); }
    float GetBaseZoom() const { return _scale; }

    /**
     * @brief 
     * @param zoom The zoom level of the camera.
     */
    void SetZoom(float zoom);
    void Zoom(float zoom) { SetZoom(_zoom + zoom); }
    float GetZoom() const { return _zoom; }

    void SetTranslation(float x, float y) { _translation = glm::vec2(x, y); _RecalculateViewProjection(); }
    void SetTranslation(const glm::vec2& translation) { SetTranslation(translation.x, translation.y); }
    void Translate(float x, float y) { SetTranslation(_translation.x + x, _translation.y + y); }
    void Translate(const glm::vec2& translation) { Translate(translation.x, translation.y); }
    const glm::vec2& GetTranslation() const { return _translation; }

    float GetWorldX() const { return _translation.x; }
    float GetWorldY() const { return _translation.y; }
    float GetWorldWidth() const { return _width / _scale; }
    float GetWorldHeight() const { return _height / _scale; }

    void SetRotation(float radian) { _rotation = radian; _RecalculateViewProjection(); }
    void Rotate(float radian) { SetRotation(_rotation + radian); }
    float GetRotation() const { return _rotation; }

    /**
     * @brief Center the given location in world. 
     */
    void CenterWorldPoint(float x, float y);
    void CenterWorldPoint(const Physics::Vector3 &point) { CenterWorldPoint(static_cast<float>(point.X), static_cast<float>(point.Y)); }

    /**
     * @brief
     * Translate screen position to world position.
     * @param x x pos of the screen coordinate
     * @param y inverse y pos of the screen coordinate
     * @return the coordinate in the world
     */
    Physics::Vector3 ScreenToWorld(float x, float y) const;

    /**
     * @brief Reset translation, zoom and rotation of the camera.
     */
    void Reset();

private:
    void _RecalculateViewProjection();

private:
    /**
     * @brief The projection area of the scene on the screen.
     */
    float _width = 1.f;
    float _height = 1.f;

    
    float _scale = 1.f;

    /**
     * @brief
     * The extra zoom level that can be applied to the camera.
     *
     * @note
     * This zoom's origin is the center of the camera. > 1 will zoom in (show less
     * world), < 1 will zoom out (show more world).
     */
    float _zoom = 1.f;

    /**
     * @brief We can rotate the camera.
     */
    float _rotation = 0.f;

    /**
     * @brief Translation of the camera in the world.
     * @note This is the camera's bottom-left corner position in the world.
     */
    glm::vec2 _translation = glm::vec2(0.f);

    glm::mat4 _view;
    glm::mat4 _viewProjection;
};
// clang-format off

DGEX_END
