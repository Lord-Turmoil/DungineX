/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Timer.h                                   *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
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

#pragma once

#include "DgeX/Defines.h"

DGEX_BEGIN

using timestamp_t = float;

/**
 * @brief Get the elapsed time since start of the game in seconds.
 *
 * @return Current timestamp in seconds.
 */
DGEX_API timestamp_t GetTimestamp();

/**
 * @brief Get the elapsed time since start time in seconds.
 *
 * Ensure that start time is obtained from GetTimestamp().
 *
 * @param startTime Start time.
 * @return Elapsed time from start time.
 */
DGEX_API timestamp_t GetElapsedTime(timestamp_t startTime);

/**
 * @brief A utility structure to represent a delta time.
 */
class DeltaTime
{
public:
    DeltaTime(timestamp_t time);

    /**
     * @brief Implicit conversion to timestamp_t in seconds.
     */
    operator timestamp_t() const;

    timestamp_t Seconds() const;
    timestamp_t Milliseconds() const;

private:
    timestamp_t _time;
};

DGEX_END
