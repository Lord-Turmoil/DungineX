/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : EventAdapter.h                            *
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
 * Convert SDL event to DungineX event.                                       *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include "DgeX/Utils/Types.h"

#include <SDL3/SDL.h>

DGEX_BEGIN

class Event;

/**
 * @brief Convert SDL event to DungineX event.
 *
 * @param event SDL Event to convert.
 * @return DungineX event.
 */
Ref<Event> ConvertSdlEvent(const SDL_Event* event);

DGEX_END
