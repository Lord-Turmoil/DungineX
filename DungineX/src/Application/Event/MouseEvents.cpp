/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : MouseEvent.cpp                            *
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

#include "DgeX/Application/Event/MouseEvents.h"

#include <sstream>

DGEX_BEGIN

MouseMovedEvent::MouseMovedEvent(FPoint position) : _position(position)
{
}

FPoint MouseMovedEvent::GetPosition() const
{
    return _position;
}

std::string MouseMovedEvent::ToString() const
{
    std::stringstream ss;
    ss << GetName() << "[x=" << _position.X << ", y=" << _position.Y << "]";
    return ss.str();
}

MouseScrolledEvent::MouseScrolledEvent(float offset) : _offset(offset)
{
}

float MouseScrolledEvent::GetOffset() const
{
    return _offset;
}

std::string MouseScrolledEvent::ToString() const
{
    std::stringstream ss;
    ss << GetName() << "[offset=" << _offset << "]";
    return ss.str();
}

MouseCode MouseButtonEvent::GetMouseCode() const
{
    return _code;
}

MouseButtonEvent::MouseButtonEvent(MouseCode code) : _code(code)
{
}

MouseButtonPressedEvent::MouseButtonPressedEvent(MouseCode code, bool repeat) : MouseButtonEvent(code), _repeat(repeat)
{
}

bool MouseButtonPressedEvent::IsRepeat() const
{
    return _repeat;
}

std::string MouseButtonPressedEvent::ToString() const
{
    std::stringstream ss;
    ss << GetName() << "[code=" << _code << ", repeat=" << _repeat << "]";
    return ss.str();
}

MouseButtonReleasedEvent::MouseButtonReleasedEvent(MouseCode code) : MouseButtonEvent(code)
{
}

std::string MouseButtonReleasedEvent::ToString() const
{
    std::stringstream ss;
    ss << GetName() << "[code=" << _code << "]";
    return ss.str();
}

DGEX_END
