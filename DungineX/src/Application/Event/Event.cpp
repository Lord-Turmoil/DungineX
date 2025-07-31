/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Event.cpp                                 *
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
 * Base event type.                                                           *
 ******************************************************************************/

#include "DgeX/Application/Event/Event.h"

DGEX_BEGIN

Event::Event() : _handled(false)
{
}

std::string Event::ToString() const
{
    return GetName();
}

bool Event::IsHandled() const
{
    return _handled;
}

void Event::SetHandled(bool handled)
{
    _handled = handled;
}

DGEX_END
