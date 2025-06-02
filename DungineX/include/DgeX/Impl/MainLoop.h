/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : MainLoop.h                                *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 2, 2025                              *
 *                                                                            *
 *                    Last Update : June 2, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Main loop of the game.                                                     *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

DGEX_BEGIN

typedef bool (*OnUpdateCallback)(void);
typedef void (*OnEventCallback)(void);

/**
 * @brief Run the main loop.
 *
 * @param onUpdate Called on frame update.
 * @param onEvent Called on receiving new events.
 */
void MainLoop(OnUpdateCallback onUpdate, OnEventCallback onEvent);

DGEX_END
