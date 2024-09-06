#include "DgeX/Application/Window.h"

#include "DgeX/Platform/OpenGL/OpenGLWindow.h"

DGEX_BEGIN

Scope<Window> Window::Create(const WindowProps &props)
{
#ifdef DGEX_GLFW
    return CreateScope<OpenGLWindow>(props);
#elif DGEX_EASYX
    DGEX_ASSERT(false, "Not implemented");
#else
    DGEX_ASSERT(false, "Unknown platform!");
#endif
}

DGEX_END
