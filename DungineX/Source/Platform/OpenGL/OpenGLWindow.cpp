#include "DgeX/Platform/OpenGL/OpenGLWindow.h"

DGEX_BEGIN

static int sWindowCount = 0;

static void GLFWErrorCallback(int error, const char *description)
{
    DGEX_LOG_ERROR("GLFW Error ({}): {}", error, description);
}

OpenGLWindow::OpenGLWindow(const WindowProps &props) : _window(nullptr)
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

void *OpenGLWindow::GetNativeWindow() const
{
    // TODO
    return nullptr;
}

void OpenGLWindow::_Init(const WindowProps &props)
{
    DGEX_LOG_INFO("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

    _data.Title = props.Title;
    _data.Width = props.Width;
    _data.Height = props.Height;

    if (sWindowCount == 0)
    {
        int ret = glfwInit();
        DGEX_ASSERT(ret, "Failed to initialize GLFW");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    _window = glfwCreateWindow(props.Width, props.Height, _data.Title.c_str(), nullptr, nullptr);
    sWindowCount++;

    // TODO: init context

    glfwSetWindowUserPointer(_window, &_data);
    SetVSync(true);
}

void OpenGLWindow::_Shutdown()
{
}

DGEX_END
