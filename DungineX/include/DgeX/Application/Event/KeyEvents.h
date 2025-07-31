/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : KeyEvent.h                                *
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

#pragma once

#include "DgeX/Application/Event/Event.h"

#include "DgeX/Device/Input/KeyCodes.h"

DGEX_BEGIN

class KeyEvent : public Event
{
public:
    DGEX_API KeyCode GetKeyCode() const;

protected:
    explicit KeyEvent(KeyCode code);

    KeyCode _code;
};

class KeyPressedEvent final : public KeyEvent
{
public:
    KeyPressedEvent(KeyCode code, bool repeat);

    DECL_EVENT_CLASS_TYPE(KeyPressed)

    DGEX_API std::string ToString() const override;

    /**
     * @brief Check if the key is a repeat event.
     *
     * If the key is pressed and hold, then the first event will be regular
     * press, and the subsequent events will be repeat events.
     *
     * @return Whether the key is repeat or not.
     */
    DGEX_API bool IsRepeat() const;

private:
    bool _repeat;
};

class KeyReleasedEvent final : public KeyEvent
{
public:
    KeyReleasedEvent(KeyCode code);

    DECL_EVENT_CLASS_TYPE(KeyReleased)

    DGEX_API std::string ToString() const override;
};

DGEX_END
