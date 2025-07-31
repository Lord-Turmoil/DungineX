/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : KeyEvent.cpp                              *
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
 * Keyboard event types.                                                      *
 ******************************************************************************/

#include "DgeX/Application/Event/KeyEvents.h"

#include <sstream>

DGEX_BEGIN

KeyCode KeyEvent::GetKeyCode() const
{
    return _code;
}

KeyEvent::KeyEvent(KeyCode code) : _code(code)
{
}

KeyPressedEvent::KeyPressedEvent(KeyCode code, bool repeat) : KeyEvent(code), _repeat(repeat)
{
}

std::string KeyPressedEvent::ToString() const
{
    std::stringstream ss;
    ss << GetName() << " [code=" << _code << ", repeat=" << _repeat << "]";
    return ss.str();
}

bool KeyPressedEvent::IsRepeat() const
{
    return _repeat;
}

KeyReleasedEvent::KeyReleasedEvent(KeyCode code) : KeyEvent(code)
{
}

std::string KeyReleasedEvent::ToString() const
{
    std::stringstream ss;
    ss << GetName() << " [code=" << _code << "]";
    return ss.str();
}

DGEX_END
