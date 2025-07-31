/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : MouseEvent.h                              *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : August 15, 2025                           *
 *                                                                            *
 *                    Last Update : August 15, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Mouse event types.                                                         *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/Event/Event.h"

#include "DgeX/Device/Input/KeyCodes.h"
#include "DgeX/Utils/Types.h"

DGEX_BEGIN

class MouseMovedEvent final : public Event
{
public:
    MouseMovedEvent(FPoint position);

    DECL_EVENT_CLASS_TYPE(MouseMoved)

    DGEX_API FPoint GetPosition() const;

    DGEX_API std::string ToString() const override;

private:
    FPoint _position;
};

class MouseScrolledEvent final : public Event
{
public:
    MouseScrolledEvent(float offset);

    DECL_EVENT_CLASS_TYPE(MouseScrolled)

    DGEX_API float GetOffset() const;

    DGEX_API std::string ToString() const override;

private:
    float _offset;
};

class MouseButtonEvent : public Event
{
public:
    DGEX_API MouseCode GetMouseCode() const;

protected:
    MouseButtonEvent(MouseCode code);

    MouseCode _code;
};

class MouseButtonPressedEvent final : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(MouseCode code, bool repeat);

    DECL_EVENT_CLASS_TYPE(MouseButtonPressed)

    DGEX_API bool IsRepeat() const;

    DGEX_API std::string ToString() const override;

private:
    bool _repeat;
};

class MouseButtonReleasedEvent final : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(MouseCode code);

    DECL_EVENT_CLASS_TYPE(MouseButtonReleased)

    DGEX_API std::string ToString() const override;
};

DGEX_END
