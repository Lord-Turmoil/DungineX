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
#include "DgeX/Utils/Macros.h"
#include "DgeX/Utils/Types.h"

#include <SDL3/SDL.h>

#include <string>

DGEX_BEGIN

using WindowFlags = unsigned char;

// clang-format off
enum : unsigned char
{
    DGEX_Window_AlwaysOnTop        = DGEX_BIT(0),    // always on top
    DGEX_Window_Borderless         = DGEX_BIT(1),    // launch as borderless window
    DGEX_Window_FullScreen         = DGEX_BIT(2),    // launch in fullscreen
    DGEX_Window_Resizable          = DGEX_BIT(3),    // allow window to resize
    DGEX_Window_VSync              = DGEX_BIT(4),    // enable vertical synchronization
    DGEX_Window_Default            = DGEX_Window_VSync
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

/**
 * Window class uses RAII to hold window resource.
 *
 * This is not intended to expose to users.
 */
class Window
{
public:
    DGEX_API ~Window();

    /**
     * @brief Render the window on the screen.
     *
     * SDL use back buffer, so all render commands will
     * be displayed only when explicitly rendered.
     */
    DGEX_API void Render() const;

    /**
     * @brief Get the native window.
     */
    DGEX_API SDL_Window* GetNativeWindow() const;

    /**
     * @brief Get the native renderer.
     */
    DGEX_API SDL_Renderer* GetNativeRenderer() const;

public:
    /**
     * @brief Create a window instance.
     *
     * User should call CreateWindow instead of this function.
     *
     * @param properties Window properties.
     */
    static Expected<Ptr<Window>, dgex_error_t> Create(const WindowProperties& properties);

private:
    Window(const WindowProperties& properties, dgex_error_t* result);

    SDL_Window* _window;
    SDL_Renderer* _renderer;
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
 * @brief Create a window with properties hint.
 */
DGEX_API Scope<Window> CreateWindow();

DGEX_END
