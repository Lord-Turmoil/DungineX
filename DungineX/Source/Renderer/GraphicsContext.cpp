#include <GLFW/glfw3.h>

#include "DgeX/Platform/OpenGL/OpenGLGraphicsContext.h"
#include "DgeX/Renderer/GraphicsContext.h"

DGEX_BEGIN

Scope<GraphicsContext> GraphicsContext::Create(void* window)
{
#if DGEX_OPENGL
    return CreateScope<OpenGLGraphicsContext>(static_cast<GLFWwindow*>(window));
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

DGEX_END
