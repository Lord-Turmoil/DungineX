#include "DgeX/Platform/OpenGL/OpenGLWindow.h"
#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Event/KeyEvent.h"
#include "DgeX/Application/Event/MouseEvent.h"
#include "DgeX/Renderer/RenderCommand.h"

#ifdef DGEX_OPENGL

DGEX_BEGIN

static int sWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
    DGEX_LOG_ERROR("GLFW Error ({}): {}", error, description);
}

OpenGLWindow::OpenGLWindow(const WindowProps& props) : _window(nullptr)
{
    _Init(props);
}

OpenGLWindow::~OpenGLWindow()
{
    _Shutdown();
}

void OpenGLWindow::Attach()
{
    glfwMakeContextCurrent(_window);
}

void OpenGLWindow::Detach()
{
    glfwMakeContextCurrent(nullptr);
}

void OpenGLWindow::OnUpdate()
{
    glfwWaitEvents();
}

void OpenGLWindow::OnRender()
{
    _context->SwapBuffers();
}

void OpenGLWindow::SetVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    if (enabled)
    {
        _data.Flags |= VSync;
    }
    else
    {
        _data.Flags &= ~VSync;
    }
}

void* OpenGLWindow::GetNativeWindow() const
{
    return _window;
}

void OpenGLWindow::_Init(const WindowProps& props)
{
    DGEX_LOG_INFO("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

    _data.Title = props.Title;
    _data.Width = props.Width;
    _data.Height = props.Height;
    _data.Flags = props.Flags;
    _data.AspectRatio = static_cast<float>(props.Width) / static_cast<float>(props.Height);

    if (sWindowCount == 0)
    {
        int ret = glfwInit();
        DGEX_ASSERT(ret, DGEX_MSG_GLFW_INIT_FAILED);
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    // Set window hints before creating one
    if (_data.Flags & Borderless)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }
    if (_data.Flags & DisableResize)
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
    if (_data.Flags & AlwaysOnTop)
    {
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    }

    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (_data.Flags & FullScreen)
    {
        _data.Width = mode->width;
        _data.Height = mode->height;
        if (_data.Flags & Borderless)
        {
            // launch in fullscreen borderless mode
            _window = glfwCreateWindow(_data.Width, _data.Height, _data.Title.c_str(), nullptr, nullptr);
            glfwSetWindowPos(_window, 0, 0);
        }
        else
        {
            // takes over the monitor
            _window = glfwCreateWindow(_data.Width, _data.Height, _data.Title.c_str(), monitor, nullptr);
        }
    }
    else
    {
        // normal window
        _window = glfwCreateWindow(_data.Width, _data.Height, _data.Title.c_str(), nullptr, nullptr);
        glfwSetWindowPos(_window, (mode->width - _data.Width) / 2, (mode->height - _data.Height) / 2);
    }
    sWindowCount++;

    _context = GraphicsContext::Create(_window);
    _context->Init();

    glfwSetWindowUserPointer(_window, &_data);
    SetVSync(_data.Flags & VSync);

    _InitEventCallback();
}

void OpenGLWindow::_InitEventCallback() const
{
    // Set GLFW callbacks
    glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.Width = width;
        data.Height = height;
        data.EventCallback(WindowResizeEvent::Create(width, height));
    });

    glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.EventCallback(WindowCloseEvent::Create());
    });

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
        case GLFW_RELEASE:
            data.EventCallback(KeyReleasedEvent::Create(static_cast<KeyCode>(key)));
            break;
        case GLFW_PRESS:
            data.EventCallback(KeyPressedEvent::Create(static_cast<KeyCode>(key), false));
            break;
        case GLFW_REPEAT:
            data.EventCallback(KeyPressedEvent::Create(static_cast<KeyCode>(key), true));
            break;
        default:
            break;
        }
    });

    glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.EventCallback(KeyTypedEvent::Create(static_cast<KeyCode>(keycode)));
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
        case GLFW_PRESS:
            data.EventCallback(MouseButtonPressedEvent::Create(button));
            break;
        case GLFW_RELEASE:
            data.EventCallback(MouseButtonReleasedEvent::Create(button));
            break;
        default:
            break;
        }
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.EventCallback(MouseScrolledEvent::Create(xOffset, yOffset));
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.EventCallback(MouseMovedEvent::Create(xPos, yPos, data.Height - yPos));
    });
}

void OpenGLWindow::_Shutdown()
{
    DGEX_LOG_INFO("Destroying window {}", _data.Title);

    glfwDestroyWindow(_window);
    sWindowCount--;
    if (sWindowCount == 0)
    {
        glfwTerminate();
    }
}

DGEX_END

#endif
