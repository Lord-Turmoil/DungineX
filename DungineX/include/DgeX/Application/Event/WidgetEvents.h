/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetEvents.h                            *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : October 4, 2025                           *
 *                                                                            *
 *                    Last Update : October 4, 2025                           *
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

class MouseClickEvent final : public Event
{
public:
    MouseClickEvent() = default;

    DECL_EVENT_CLASS_TYPE(MouseClick)
};

DGEX_END
