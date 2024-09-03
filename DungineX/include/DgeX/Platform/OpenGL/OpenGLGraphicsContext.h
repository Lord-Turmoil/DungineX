#pragma once

#include "DgeX/Renderer/GraphicsContext.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

DGEX_BEGIN

class OpenGLGraphicsContext final : public GraphicsContext
{
public:
    OpenGLGraphicsContext(GLFWwindow* window);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* _window;
};

DGEX_END
