#pragma once

#include "DgeX/Scene/Camera/Camera.h"

DGEX_BEGIN

/**
 * @brief
 * Interface camera is intended to be used for the UI to draw widgets.
 * For now, this camera is fixed to the screen size, and automatically
 * adjusts the projection matrix when the screen size changes.
 *
 * @note
 * It uses the same coordinate system as the screen, which is (0, 0) in the
 * top-left corner, and (width, height) in the bottom-right corner.
 *
 * @note
 * It also uses a z-index to support the depth of the widgets. The z-index
 * ranges from -1000 to 1000, where -1000 is the back and 1000 is the front.
 */
class InterfaceCamera : public Camera
{
public:
    InterfaceCamera(float width, float height);

    void OnResize(float width, float height);
};

DGEX_END
