#include "DgeX/Application/Window.h"

#include "DgeX/Platform/OpenGL/OpenGLWindow.h"

DGEX_BEGIN

Scope<Window> Window::Create(const WindowProps& props)
{
#if DGEX_OPENGL
    return CreateScope<OpenGLWindow>(props);
#else
    DGEX_ASSERT(false, DGEX_MSG_UNKNOWN_PLATFORM);
#endif
}

DGEX_END
