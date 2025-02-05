#pragma once

#include "DgeX/Application/Event/Event.h"
#include "DgeX/Application/Input/MouseCode.h"

#include <sstream>

DGEX_BEGIN

class MouseMovedEvent final : public Event
{
public:
    MouseMovedEvent(const double x, const double y) : _position({ x, y })
    {
    }

    double GetX() const
    {
        return _position.X;
    }

    double GetY() const
    {
        return _position.Y;
    }

    const MousePosition& GetMousePosition() const
    {
        return _position;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _position.X << ", " << _position.Y;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseMoved)
    DECL_EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    static Ref<MouseMovedEvent> Create(const double x, const double y)
    {
        return CreateRef<MouseMovedEvent>(x, y);
    }

private:
    MousePosition _position;
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

/**
 * @brief This is a pseudo-event , as OpenGL does not support mouse click events.
 */
class MouseButtonClickedEvent : public MouseButtonEvent
{
public:
    MouseButtonClickedEvent(MouseCode code) : MouseButtonEvent(code)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << GetName() << " Event: " << _mouseCode;
        return ss.str();
    }

    DECL_EVENT_CLASS_TYPE(MouseButtonClicked)

    static Ref<MouseButtonClickedEvent> Create(MouseCode code)
    {
        return CreateRef<MouseButtonClickedEvent>(code);
    }
};

DGEX_END
