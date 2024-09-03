#pragma once

#include "DgeX/Application/Event/Event.h"
#include "DgeX/Application/Input/MouseCode.h"

DGEX_BEGIN

class MouseMovedEvent final : public Event
{
public:
    MouseMovedEvent(const double x, const double y, const double inverseY) : _x(x), _y(y), _inverseY(inverseY)
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

    double GetInverseY() const
    {
        return _inverseY;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _x << ", " << _y << ", " << _inverseY;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseMoved)
    DECL_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    static Ref<MouseMovedEvent> Create(const double x, const double y, const double inverseY)
    {
        return CreateRef<MouseMovedEvent>(x, y, inverseY);
    }

private:
    double _x;
    double _y;
    double _inverseY;
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

    static Ref<MouseScrolledEvent> Create(const double xOffset, const double yOffset)
    {
        return CreateRef<MouseScrolledEvent>(xOffset, yOffset);
    }

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
    MouseButtonPressedEvent(MouseCode code) : MouseButtonEvent(code)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _mouseCode;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseButtonPressed)

    static Ref<MouseButtonPressedEvent> Create(MouseCode code)
    {
        return CreateRef<MouseButtonPressedEvent>(code);
    }
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(MouseCode code) : MouseButtonEvent(code)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _mouseCode;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseButtonReleased)

    static Ref<MouseButtonReleasedEvent> Create(MouseCode code)
    {
        return CreateRef<MouseButtonReleasedEvent>(code);
    }
};

DGEX_END
