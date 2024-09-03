#pragma once

#include "DgeX/Application/Event/Event.h"

#include <sstream>

DGEX_BEGIN

class WindowEvent : public Event
{
public:
    DECL_EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class WindowResizeEvent final : public WindowEvent
{
public:
    WindowResizeEvent(int width, int height) : _width(width), _height(height)
    {
    }

    int GetWidth() const
    {
        return _width;
    }

    int GetHeight() const
    {
        return _height;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _width << ", " << _height;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(WindowResize)

    static Ref<WindowResizeEvent> Create(int width, int height)
    {
        return CreateRef<WindowResizeEvent>(width, height);
    }

private:
    int _width;
    int _height;
};

class WindowCloseEvent final : public WindowEvent
{
public:
    DECL_EVENT_CLASS_TYPE(WindowClose)

    static Ref<WindowCloseEvent> Create()
    {
        return CreateRef<WindowCloseEvent>();
    }
};

class InterfaceEvent : public Event
{
public:
    DECL_EVENT_CLASS_CATEGORY(EventCategoryInterface)
};

/**
 * @brief Require transition to another interface.
 * @note If fromName is empty, it means directly open the interface.
 */
class InterfaceTransitEvent final : public InterfaceEvent
{
public:
    InterfaceTransitEvent(std::string fromName, std::string toName, std::string style)
        : _fromName(std::move(fromName)), _toName(std::move(toName)), _style(std::move(style))
    {
    }

    InterfaceTransitEvent(std::string toName, std::string style) : _toName(std::move(toName)), _style(std::move(style))
    {
    }

    InterfaceTransitEvent(std::string toName) : _toName(std::move(toName))
    {
    }

    const std::string& GetFromName() const
    {
        return _fromName;
    }

    const std::string& GetToName() const
    {
        return _toName;
    }

    const std::string& GetStyle() const
    {
        return _style;
    }

    DECL_EVENT_CLASS_TYPE(InterfaceTransit)

    static InterfaceTransitEvent Create(std::string fromName, std::string toName, std::string style)
    {
        return InterfaceTransitEvent(std::move(fromName), std::move(toName), std::move(style));
    }

    static InterfaceTransitEvent Create(std::string toName, std::string style)
    {
        return InterfaceTransitEvent(std::move(toName), std::move(style));
    }

    static InterfaceTransitEvent Create(std::string toName)
    {
        return InterfaceTransitEvent(std::move(toName));
    }

private:
    std::string _fromName;
    std::string _toName;
    std::string _style;
};

class InterfaceChangeEvent final : public InterfaceEvent
{
public:
    InterfaceChangeEvent(std::string fromName, std::string toName, std::string style)
        : _fromName(std::move(fromName)), _toName(std::move(toName)), _style(std::move(style))
    {
    }

    InterfaceChangeEvent(std::string toName, std::string style) : _toName(std::move(toName)), _style(std::move(style))
    {
    }

    InterfaceChangeEvent(std::string toName) : _toName(std::move(toName))
    {
    }

    const std::string& GetFromName() const
    {
        return _fromName;
    }

    const std::string& GetToName() const
    {
        return _toName;
    }

    const std::string& GetStyle() const
    {
        return _style;
    }

    DECL_EVENT_CLASS_TYPE(InterfaceChange)

    static Ref<InterfaceChangeEvent> Create(std::string fromName, std::string toName, std::string style)
    {
        return CreateRef<InterfaceChangeEvent>(std::move(fromName), std::move(toName), std::move(style));
    }

    static Ref<InterfaceChangeEvent> Create(std::string toName, std::string style)
    {
        return CreateRef<InterfaceChangeEvent>(std::move(toName), std::move(style));
    }

    static Ref<InterfaceChangeEvent> Create(std::string toName)
    {
        return CreateRef<InterfaceChangeEvent>(std::move(toName));
    }

private:
    std::string _fromName;
    std::string _toName;
    std::string _style;
};

class InterfaceCloseEvent final : public InterfaceEvent
{
public:
    InterfaceCloseEvent(bool transit = false);

    bool RequireTransit() const
    {
        return _transit;
    }

    DECL_EVENT_CLASS_TYPE(InterfaceTransit)

    static Ref<InterfaceCloseEvent> Create(bool transit = false)
    {
        return CreateRef<InterfaceCloseEvent>(transit);
    }

private:
    bool _transit;
};

DGEX_END
