/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : EventTypes.h                              *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
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

DGEX_BEGIN

// clang-format off
enum class EventType
{
    None,
    // Keyboard events
    KeyPressed, KeyReleased,
    // Mouse events
    MouseMoved, MouseScrolled, MouseButtonPressed, MouseButtonReleased,
    // Window events
    WindowClose,
};
// clang-format on

DGEX_END
