/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Timer.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : August 24, 2025                           *
 *                                                                            *
 *                    Last Update : August 24, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Timer.                                                                     *
 ******************************************************************************/

#include "DgeX/Core/Timer.h"

#include <SDL3/SDL.h>

DGEX_BEGIN

timestamp_t GetTimestamp()
{
    return static_cast<timestamp_t>(SDL_GetTicks()) / 1000.0f;
}

timestamp_t GetElapsedTime(timestamp_t startTime)
{
    return GetTimestamp() - startTime;
}

DGEX_END
