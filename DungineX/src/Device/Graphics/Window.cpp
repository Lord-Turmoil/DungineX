/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Window.cpp                                *
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

#include "DgeX/Device/Graphics/Window.h"

#include "DgeX/Error.h"
#include "DgeX/Utils/Assert.h"
#include "DgeX/Utils/Log.h"

#include <utility>

DGEX_BEGIN

static WindowProperties sWindowPropertiesHint("DungineX", 640, 480, DgexWindowDefault);
static SDL_Window* sNativeWindow = nullptr;

static SDL_WindowFlags GetSdlWindowFlags(WindowFlags flags)
{
    SDL_WindowFlags sdlFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
    if (flags & DgexWindowAlwaysOnTop)
    {
        sdlFlags |= SDL_WINDOW_ALWAYS_ON_TOP;
    }
    if (flags & DgexWindowBorderless)
    {
        sdlFlags |= SDL_WINDOW_BORDERLESS;
    }
    if (flags & DgexWindowFullScreen)
    {
        sdlFlags |= SDL_WINDOW_FULLSCREEN;
    }
    if (flags & DgexWindowResizable)
    {
        sdlFlags |= SDL_WINDOW_RESIZABLE;
    }

    // SDL flags doesn't contain vsync option, will be handled separately.

    return sdlFlags;
}

WindowProperties::WindowProperties(std::string title, int width, int height, WindowFlags flags)
    : Title(std::move(title)), Width(width), Height(height), Flags(flags)
{
}

void SetWindowPropertiesHint(const WindowProperties& properties)
{
    sWindowPropertiesHint = properties;
}

dgex_error_t InitWindow()
{
    // clang-format off
    SDL_Window* window = SDL_CreateWindow(
        sWindowPropertiesHint.Title.c_str(),
        sWindowPropertiesHint.Width,
        sWindowPropertiesHint.Height,
        GetSdlWindowFlags(sWindowPropertiesHint.Flags));
    // clang-format on

    if (!window)
    {
        DGEX_CORE_ERROR("Failed to create window: {0}", SDL_GetError());
        return DGEX_ERROR_WINDOW_INIT;
    }

    sNativeWindow = window;

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    DGEX_CORE_DEBUG("Window size: {0}x{1}", width, height);

    SDL_ShowWindow(sNativeWindow);

    DGEX_CORE_DEBUG("Window initialized");

    return DGEX_SUCCESS;
}

void DestroyWindow()
{
    DGEX_ASSERT(sNativeWindow, "Window not initialized");

    SDL_DestroyWindow(sNativeWindow);

    DGEX_CORE_DEBUG("Window destroyed");
}

SDL_Window* GetNativeWindow()
{
    DGEX_ASSERT(sNativeWindow, "Window not initialized");

    return sNativeWindow;
}

DGEX_END
