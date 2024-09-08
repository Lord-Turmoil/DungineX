#pragma once

#include <sstream>
#include "DgeX/Application/Event/Event.h"
#include "DgeX/Application/Input/MouseCode.h"
#include "DgeX/Common/Base.h"

DGEX_BEGIN

class MouseMovedEvent final : public Event
{
public:
    MouseMovedEvent(const double x, const double y) : _x(x), _y(y)
    {
    }

    double GetX() const
    {
        return _x;
    }

    double GetY() const
    {
        return _y;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _x << ", " << _y;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseMoved)
    DECL_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    double _x;
    double _y;
};

class MouseScrolledEvent final : public Event
{
public:
    MouseScrolledEvent(const double xOffset, const double yOffset) : _offsetX(xOffset), _offsetY(yOffset)
    {
    }

    double GetOffsetX() const
    {
        return _offsetX;
    }

    double GetOffsetY() const
    {
        return _offsetY;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << GetOffsetX() << ", " << GetOffsetY();
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseScrolled)
    DECL_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
    double _offsetX, _offsetY;
};

class MouseButtonEvent : public Event
{
public:
    MouseCode GetMouseCode() const
    {
        return _mouseCode;
    }

    DECL_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

protected:
    explicit MouseButtonEvent(const MouseCode code) : _mouseCode(code)
    {
    }

    MouseCode _mouseCode;
};

class MouseButtonPressedEvent final : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(const MouseCode code) : MouseButtonEvent(code)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _mouseCode;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(const MouseCode code) : MouseButtonEvent(code)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _mouseCode;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseButtonReleased)
};

DGEX_END
