/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Input.h                                   *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : July 31, 2025                             *
 *                                                                            *
 *                    Last Update : July 31, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Direct input handling.                                                     *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Device/Input/KeyCode.h"
#include "DgeX/Utils/Types.h"

DGEX_BEGIN

/**
 * @brief Checks if a specific key is currently pressed.
 *
 * @param key The key code to check.
 * @return true if the key is pressed, false otherwise.
 */
DGEX_API bool IsKeyPressed(KeyCode key);

/**
 * @brief Checks if a specific mouse button is currently pressed.
 *
 * @param button The mouse button code to check.
 * @return true if the mouse button is pressed, false otherwise.
 */
DGEX_API bool IsMousePressed(MouseCode button);

/**
 * @brief Get the current position of the mouse cursor.
 *
 * @return The position of the mouse from the top-left corner of the window.
 */
DGEX_API FPoint GetMousePosition();

DGEX_END
