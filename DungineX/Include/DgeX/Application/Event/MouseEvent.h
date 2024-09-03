#pragma once

#include "DgeX/Common/Base.h"
#include "DgeX/Application/Event/Event.h"
#include "DgeX/Application/Input/MouseCode.h"

DGEX_BEGIN

class MouseMovedEvent final : public Event
{
public:
    MouseMovedEvent(const double x, const double y) : _x(x), _y(y)
    {
    }


    double GetX() const { return _x; }
    double GetY() const { return _y; }


    std::wstring ToString() const override
    {
        std::wstringstream ss;
        ss << GetName() << L" Event: " << _x << L", " << _y;
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
    MouseScrolledEvent(const float xOffset, const float yOffset)
        : _offsetX(xOffset), _offsetY(yOffset)
    {
    }


    double GetOffsetX() const { return _offsetX; }
    double GetOffsetY() const { return _offsetY; }


    std::wstring ToString() const override
    {
        std::wstringstream ss;
        ss << GetName() << L" Event: " << GetOffsetX() << L", " << GetOffsetY();
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
    MouseCode GetMouseCode() const { return _mouseCode; }

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
    MouseButtonPressedEvent(const MouseCode code, const bool isRepeat = false)
        : MouseButtonEvent(code), _isRepeat(isRepeat)
    {
    }


    bool IsRepeat() const { return _isRepeat; }


    std::wstring ToString() const override
    {
        std::wstringstream ss;
        ss << GetName() << L" Event: " << _mouseCode;
        return ss.str();
    }


    DECL_EVENT_CLASS_TYPE(MouseButtonPressed)

private:
    bool _isRepeat;
};


class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(const MouseCode code) : MouseButtonEvent(code)
    {
    }


    std::wstring ToString() const override
    {
        std::wstringstream ss;
        ss << GetName() << L" Event: " << _mouseCode;
        return ss.str();
    }


    DECL_EVENT_CLASS_TYPE(MouseButtonReleased)
};


DGEX_END
