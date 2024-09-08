#include "DgeX/Platform/OpenGL/OpenGLWindow.h"

#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Event/KeyEvent.h"
#include "DgeX/Application/Event/MouseEvent.h"
#include "DgeX/Renderer/RenderApi.h"

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

void OpenGLWindow::OnUpdate()
{
    glfwPollEvents();
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

    if (_data.Flags & FullScreen)
    {
        auto monitor = glfwGetPrimaryMonitor();
        if (_data.Flags & Borderless)
        {
            // launch in fullscreen borderless mode
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            _data.Width = mode->width;
            _data.Height = mode->height;
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
    }
    sWindowCount++;

    _context = GraphicsContext::Create(_window);
    _context->Init();

    glfwSetWindowUserPointer(_window, &_data);
    SetVSync(_data.Flags & VSync);

    _InitEventCallback();

    RenderApi::SetViewport(0, 0, _data.Width, _data.Height);
}

void OpenGLWindow::_InitEventCallback() const
{
    // Set GLFW callbacks
    glfwSetWindowSizeCallback(_window, _WindowSizeCallback);

    glfwSetFramebufferSizeCallback(_window, _FrameBufferSizeCallback);

    glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.EventCallback(CreateRef<WindowCloseEvent>());
    });

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
        case GLFW_RELEASE:
            data.EventCallback(CreateRef<KeyReleasedEvent>(key));
            break;
        case GLFW_PRESS:
            data.EventCallback(CreateRef<KeyPressedEvent>(key, false));
            break;
        case GLFW_REPEAT:
            data.EventCallback(CreateRef<KeyPressedEvent>(key, true));
            break;
        default:
            break;
        }
    });

    glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.EventCallback(CreateRef<KeyTypedEvent>(keycode));
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action)
        {
        case GLFW_PRESS:
            data.EventCallback(CreateRef<MouseButtonPressedEvent>(button));
            break;
        case GLFW_RELEASE:
            data.EventCallback(CreateRef<MouseButtonReleasedEvent>(button));
            break;
        default:
            break;
        }
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.EventCallback(CreateRef<MouseScrolledEvent>(xOffset, yOffset));
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.EventCallback(CreateRef<MouseMovedEvent>(xPos, yPos));
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

void OpenGLWindow::_WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    data.Width = width;
    data.Height = height;

    data.EventCallback(CreateRef<WindowResizeEvent>(width, height));

    if (!(data.Flags & LockAspectRatio))
    {
        RenderApi::SetViewport(0, 0, width, height);
    }
}

/**
 * @brief Called when frame buffer size changes
 * @ref https://diegomacario.github.io/2021/04/23/how-to-keep-the-aspect-ratio-of-an-opengl-window-constant.html
 * @param window window
 * @param width width of buffer
 * @param height height of buffer
 */
void OpenGLWindow::_FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    if (!(data.Flags & LockAspectRatio))
    {
        RenderApi::SetViewport(0, 0, width, height);
        return;
    }

    float frameWidth = static_cast<float>(width);
    float frameHeight = static_cast<float>(height);
    float desiredAspectRatio = data.AspectRatio;

    int widthOfViewport;
    int heightOfViewport;
    int lowerLeftCornerOfViewportX;
    int lowerLeftCornerOfViewportY;

    float requiredHeightOfViewport = frameWidth * (1.0f / desiredAspectRatio);
    if (requiredHeightOfViewport > frameHeight)
    {
        float requiredWidthOfViewport = frameHeight * desiredAspectRatio;
        if (requiredWidthOfViewport > frameWidth)
        {
            DGEX_ASSERT(false, DGEX_MSG_INVALID_FRAME_BUFFER_SIZE);
            return;
        }

        widthOfViewport = static_cast<int>(requiredWidthOfViewport);
        heightOfViewport = static_cast<int>(frameHeight);

        lowerLeftCornerOfViewportX = static_cast<int>((frameWidth - requiredWidthOfViewport) * 0.5f);
        lowerLeftCornerOfViewportY = 0;
    }
    else
    {
        widthOfViewport = static_cast<int>(frameWidth);
        heightOfViewport = static_cast<int>(requiredHeightOfViewport);

        lowerLeftCornerOfViewportX = 0;
        lowerLeftCornerOfViewportY = static_cast<int>((frameHeight - requiredHeightOfViewport) * 0.5f);
    }

    glViewport(lowerLeftCornerOfViewportX, lowerLeftCornerOfViewportY, widthOfViewport, heightOfViewport);
}

DGEX_END
