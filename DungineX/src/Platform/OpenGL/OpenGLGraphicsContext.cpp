#include "DgeX/Platform/OpenGL/OpenGLGraphicsContext.h"

#ifdef DGEX_OPENGL

#include "DgeX/Common/Assert.h"
#include "DgeX/Utils/String.h"

#include <glad/glad.h>

DGEX_BEGIN

OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window) : _window(window)
{
    DGEX_ASSERT(_window, "Window is null");
}

void OpenGLGraphicsContext::Init()
{
    glfwMakeContextCurrent(_window);
    int status = gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));
    DGEX_ASSERT(status, DGEX_MSG_GLAD_INIT_FAILED);

    DGEX_CORE_WARN("OpenGL Info:");
    DGEX_CORE_WARN("  Vendor: {0}", Utils::String::ToString(glGetString(GL_VENDOR)));
    DGEX_CORE_WARN("  Renderer: {0}", Utils::String::ToString(glGetString(GL_RENDERER)));
    DGEX_CORE_WARN("  Version: {0}", Utils::String::ToString(glGetString(GL_VERSION)));
}

void OpenGLGraphicsContext::SwapBuffers()
{
    glfwSwapBuffers(_window);
}

DGEX_END

#endif
