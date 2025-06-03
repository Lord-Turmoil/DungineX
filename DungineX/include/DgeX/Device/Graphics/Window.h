/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Window.h                                  *
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
 * Window resource.                                                           *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Error.h"
#include "DgeX/Utils/Macros.h"

#include <SDL3/SDL.h>

#include <string>

DGEX_BEGIN

using WindowFlags = unsigned char;

// clang-format off
enum : unsigned char
{
    DgexWindowAlwaysOnTop        = DGEX_BIT(0),    // always on top
    DgexWindowBorderless         = DGEX_BIT(1),    // launch as borderless window
    DgexWindowFullScreen         = DGEX_BIT(2),    // launch in fullscreen
    DgexWindowResizable          = DGEX_BIT(3),    // allow window to resize
    DgexWindowDefault            = 0
};

// clang-format on

/**
 * Window properties.
 */
struct WindowProperties
{
    // The titled displayed.
    std::string Title;

    // Width of the window, in pixel.
    int Width;

    // Height of the window, in pixel.
    int Height;

    // Flags of window properties.
    WindowFlags Flags;

    DGEX_API WindowProperties(std::string title, int width, int height, WindowFlags flags);
};

// ============================================================================
// API
// ----------------------------------------------------------------------------

/**
 * @brief Set window properties hint.
 *
 * It works as a hint when creating the window, so it only works in
 * DgeXInit before the window is created.
 *
 * @param properties Properties of the window.
 */
DGEX_API void SetWindowPropertiesHint(const WindowProperties& properties);

/**
 * @brief Initialize window with properties hint.
 *
 * @return 0 on success, otherwise failure.
 */
dgex_error_t InitWindow();

/**
 * @brief Destroy window.
 *
 * @return 0 on success, otherwise failure.
 */
void DestroyWindow();

/**
 * @brief Get native window.
 *
 * @return Native SDL window.
 */
SDL_Window* GetNativeWindow();

DGEX_END
