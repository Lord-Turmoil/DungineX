#pragma once

#include <GLFW/glfw3.h>

#include "DgeX/Application/Window.h"
#include "DgeX/Renderer/GraphicsContext.h"

DGEX_BEGIN

class OpenGLWindow final : public Window
{
public:
    OpenGLWindow(const WindowProps& props);
    ~OpenGLWindow() override;

    void OnUpdate() override;

    int GetWidth() const override
    {
        return _data.Width;
    }

    int GetHeight() const override
    {
        return _data.Height;
    }

    void SetEventCallback(const EventCallbackFn& callback) override
    {
        _data.EventCallback = callback;
    }

    void SetVSync(bool enabled) override;

    bool IsVSync() const override
    {
        return _data.Flags & VSync;
    }

    void* GetNativeWindow() const override;

private:
    void _Init(const WindowProps& props);
    void _InitEventCallback() const;
    void _Shutdown();

    static void _WindowSizeCallback(GLFWwindow* window, int width, int height);
    static void _FrameBufferSizeCallback(GLFWwindow* window, int frameWidth, int frameHeight);

private:
    GLFWwindow* _window;
    Scope<GraphicsContext> _context;

    /**
     * @brief In OpenGL, we can bind a data structure to a window, so that
     *          we can access the data structure from the window.
     */
    struct WindowData
    {
        std::string Title;
        int Width;
        int Height;
        WindowFlags Flags;

        EventCallbackFn EventCallback;

        float AspectRatio;
    };

    WindowData _data;
};

DGEX_END
