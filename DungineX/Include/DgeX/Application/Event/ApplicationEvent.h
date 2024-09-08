#pragma once

#include "DgeX/Application/Event/Event.h"

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

private:
    int _width;
    int _height;
};

class WindowCloseEvent final : public WindowEvent
{
public:
    DECL_EVENT_CLASS_TYPE(WindowClose)
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

private:
    bool _transit;
};

DGEX_END
