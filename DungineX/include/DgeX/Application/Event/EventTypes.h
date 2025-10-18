/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : EventTypes.h                              *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : August 3, 2025                            *
 *                                                                            *
 *                    Last Update : August 15, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Event types definition.                                                    *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <string>

DGEX_BEGIN

/**
 * When the application handles events, it first collects the events at application level,
 * then dispatches them to the DOM elements. The DOM elements will process the events from
 * children to parents, and children can stop the propagation of the event by marking it
 * as handled.
 */

enum class EventType
{
    None,

    // Keyboard events
    KeyPressed,
    KeyReleased,

    // Mouse events
    MouseMoved,
    MouseScrolled,
    MouseButtonPressed,
    MouseButtonReleased,

    // Window events
    WindowClose,
    WindowResized,

    // Widget events, are fired by widgets
    // The position of the mouse can be inferred from MouseMoved event
    MouseEnter, // implies hover state, and usually should not propagate to parent
    MouseLeave, // cancel hover state
    MouseClick, // fired when mouse button is pressed and released on the same widget
};

inline std::string ToString(EventType type)
{
    switch (type)
    {
    case EventType::None:
        return "None";
    case EventType::KeyPressed:
        return "KeyPressed";
    case EventType::KeyReleased:
        return "KeyReleased";
    case EventType::MouseMoved:
        return "MouseMoved";
    case EventType::MouseScrolled:
        return "MouseScrolled";
    case EventType::MouseButtonPressed:
        return "MouseButtonPressed";
    case EventType::MouseButtonReleased:
        return "MouseButtonReleased";
    case EventType::WindowClose:
        return "WindowClose";
    case EventType::WindowResized:
        return "WindowResized";
    case EventType::MouseEnter:
        return "MouseEnter";
    case EventType::MouseLeave:
        return "MouseLeave";
    case EventType::MouseClick:
        return "MouseClick";
    }
    return "Unknown";
}

DGEX_END
