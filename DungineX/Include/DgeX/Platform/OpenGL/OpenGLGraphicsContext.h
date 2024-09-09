#pragma once

#include <GLFW/glfw3.h>
#include "DgeX/Renderer/GraphicsContext.h"

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
