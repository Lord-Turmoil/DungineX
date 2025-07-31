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

DGEX_BEGIN

class WindowCloseEvent final : public Event
{
public:
    WindowCloseEvent() = default;

    DECL_EVENT_CLASS_TYPE(WindowClose)
};

DGEX_END
