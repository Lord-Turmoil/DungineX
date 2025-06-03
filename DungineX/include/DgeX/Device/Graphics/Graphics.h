/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Graphics.h                                *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 3, 2025                              *
 *                                                                            *
 *                    Last Update : June 3, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Graphics device.                                                           *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Error.h"

DGEX_BEGIN

/**
 * @brief Initialize graphics device.
 *
 * This includes window and renderer initialization.
 *
 * @return 0 on success, otherwise failure.
 */
DGEX_API dgex_error_t InitGraphics();

/**
 * @brief Destroy graphics device.
 *
 * This will destroy window and renderer.
 *
 * @return 0 on success, otherwise failure.
 */
DGEX_API void DestroyGraphics();

DGEX_END
