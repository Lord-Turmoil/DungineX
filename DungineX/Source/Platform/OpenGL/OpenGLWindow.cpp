#include "DgeX/Platform/OpenGL/OpenGLWindow.h"

DGEX_BEGIN

static void GLFWErrorCallback(int error, const char* description)
{
    DGEX_LOG_ERROR("GLFW Error (%d): %s", error, description);
}


OpenGLWindow::OpenGLWindow(const WindowProps& props) : _window(nullptr)
{
    _Init(props);
}


OpenGLWindow::~OpenGLWindow()
{
    _Shutdown();
}


void OpenGLWindow::OnUpdate()
{

}


void OpenGLWindow::SetVSync(bool enabled)
{
}


void* OpenGLWindow::GetNativeWindow() const
{
    // TODO
    return nullptr;
}


void OpenGLWindow::_Init(const WindowProps& props)
{
    DGEX_LOG_INFO("Creating window %s (%u, %u)", props.Title.c_str(), props.Width, props.Height);

    _title = props.Title;
    _width = props.Width;
    _height = props.Height;

}


void OpenGLWindow::_Shutdown()
{

}


DGEX_END
