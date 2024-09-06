#pragma once

#include <functional>

#include "DgeX/Common/Base.h"
#include "Event/Event.h"

DGEX_BEGIN

struct WindowProps
{
    std::string Title;
    int Width;
    int Height;

    WindowProps(std::string title = "DungineX", int width = 1600, int height = 900)
        : Title(std::move(title)), Width(width), Height(height)
    {
    }
};

class Window
{
public:
    using EventCallbackFn = std::function<void(Ref<Event> &)>;

    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    // Window attributes
    virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void *GetNativeWindow() const = 0;

    static Scope<Window> Create(const WindowProps &props = WindowProps());
};

DGEX_END
