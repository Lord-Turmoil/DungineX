#include "DgeX/Renderer/GraphicsContext.h"
#include "DgeX/Renderer/OpenGL/OpenGLGraphicsContext.h"

#include <GLFW/glfw3.h>

DGEX_BEGIN

Scope<GraphicsContext> GraphicsContext::Create(void* window)
{
#ifdef DGEX_OPENGL
    return CreateScope<OpenGLGraphicsContext>(static_cast<GLFWwindow*>(window));
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

DGEX_END
