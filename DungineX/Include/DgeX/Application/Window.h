#pragma once

#include "DgeX/Application/Event/Event.h"

DGEX_BEGIN

// clang-format off
using WindowFlags = unsigned char;

enum : unsigned char
{
    Plain              = 0,
    DisableResize      = BIT(0),    // whether allow window resize
    FullScreen         = BIT(1),    // whether launch in fullscreen
    Borderless         = BIT(2),    // launch as borderless window
    AlwaysOnTop        = BIT(3),    // always on top
    LockAspectRatio    = BIT(4),    // lock aspect ratio as window resizes
    VSync              = BIT(5),    // enable vertical synchronization
    Default            = VSync
};

// clang-format on

struct WindowProps
{
    std::string Title;
    int Width;
    int Height;
    WindowFlags Flags;

    WindowProps(std::string title, int width, int height, WindowFlags flags)
        : Title(std::move(title)), Width(width), Height(height), Flags(flags)
    {
    }
};

class Window
{
public:
    Window() = default;
    Window(const Window& other) = delete;
    Window(Window&& other) noexcept = delete;
    Window& operator=(const Window& other) = delete;
    Window& operator=(Window&& other) noexcept = delete;
    virtual ~Window() = default;

    /**
     * @brief Attach window to the current thread.
     */
    virtual void Attach() = 0;

    /**
     * @brief Detach window from the current thread.
     */
    virtual void Detach() = 0;

    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    // Window attributes
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual int GetRefreshRate() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Scope<Window> Create(const WindowProps& props);
};

DGEX_END
