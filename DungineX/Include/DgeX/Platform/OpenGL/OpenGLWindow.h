#pragma once

#include <GLFW/glfw3.h>

#include "DgeX/Application/Window.h"

DGEX_BEGIN

class OpenGLWindow final : public Window
{
public:
    OpenGLWindow(const WindowProps &props);
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

    void SetEventCallback(const EventCallbackFn &callback) override
    {
        _data.EventCallback = callback;
    }

    void SetVSync(bool enabled) override;
    bool IsVSync() const override
    {
        return _data.VSync;
    }

    void *GetNativeWindow() const override;

private:
    void _Init(const WindowProps &props);
    void _Shutdown();

private:
    GLFWwindow *_window;

    /**
     * @brief In OpenGL, we can bind a data structure to a window, so that
     *          we can access the data structure from the window.
     */
    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData _data;
};

DGEX_END
