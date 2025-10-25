/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetEvents.h                            *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 4, 2025                           *
 *                                                                            *
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Widget event types.                                                        *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/Event/Event.h"

DGEX_BEGIN

class BaseWidget;

class MouseEnterEvent final : public Event
{
public:
    MouseEnterEvent() = default;

    DECL_EVENT_CLASS_TYPE(MouseEnter)
};

class MouseLeaveEvent final : public Event
{
public:
    MouseLeaveEvent() = default;

    DECL_EVENT_CLASS_TYPE(MouseLeave)
};

class MouseClickedEvent final : public Event
{
public:
    MouseClickedEvent() = default;

    DECL_EVENT_CLASS_TYPE(MouseClick)
};

DGEX_END