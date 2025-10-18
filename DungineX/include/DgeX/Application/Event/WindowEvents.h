/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : MouseEvent.h                              *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
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

class WindowResizedEvent final : public Event
{
public:
    WindowResizedEvent(int width, int height);

    DECL_EVENT_CLASS_TYPE(WindowResized)

    DGEX_API int GetWidth() const;
    DGEX_API int GetHeight() const;

private:
    int _width;
    int _height;
};

DGEX_END
