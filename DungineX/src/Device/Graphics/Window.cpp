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
#include "DgeX/Utils/Log.h"

#include <utility>

DGEX_BEGIN

static WindowProperties sWindowPropertiesHint("DungineX", 640, 480, DGEX_Window_Default);

static SDL_WindowFlags GetSdlWindowFlags(WindowFlags flags)
{
    SDL_WindowFlags sdlFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
    if (flags & DGEX_Window_AlwaysOnTop)
    {
        sdlFlags |= SDL_WINDOW_ALWAYS_ON_TOP;
    }
    if (flags & DGEX_Window_Borderless)
    {
        sdlFlags |= SDL_WINDOW_BORDERLESS;
    }
    if (flags & DGEX_Window_FullScreen)
    {
        sdlFlags |= SDL_WINDOW_FULLSCREEN;
    }
    if (flags & DGEX_Window_Resizable)
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

Window::~Window()
{
    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if (_window)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

void Window::Render() const
{
    if (!SDL_RenderPresent(_renderer))
    {
        DGEX_CORE_ERROR("Failed to render window: {0}", SDL_GetError());
    }
}

SDL_Window* Window::GetNativeWindow() const
{
    return _window;
}

SDL_Renderer* Window::GetNativeRenderer() const
{
    return _renderer;
}

Expected<Ptr<Window>, dgex_error_t> Window::Create(const WindowProperties& properties)
{
    dgex_error_t result = DGEX_SUCCESS;
    auto window = new Window(properties, &result);
    if (result == DGEX_SUCCESS)
    {
        return Success(window);
    }

    return Failure(result);
}

Window::Window(const WindowProperties& properties, dgex_error_t* result) : _window(nullptr), _renderer(nullptr)
{
    SDL_Window* window = SDL_CreateWindow(properties.Title.c_str(), properties.Width, properties.Height,
                                          GetSdlWindowFlags(properties.Flags));
    if (!window)
    {
        *result = ERROR_WINDOW_INIT;
        DGEX_CORE_ERROR(SDL_GetError());
        return;
    }
    _window = window;

    SDL_Renderer* renderer = SDL_CreateRenderer(_window, nullptr);
    if (!renderer)
    {
        *result = ERROR_RENDERER_INIT;
        DGEX_CORE_ERROR(SDL_GetError());
        return;
    }
    _renderer = renderer;

    if (!SDL_ShowWindow(_window))
    {
        *result = ERROR_WINDOW_INIT;
        DGEX_CORE_ERROR("Unable to show window: {0}", SDL_GetError());
        return;
    }

    if (properties.Flags & DGEX_Window_VSync)
    {
        if (!SDL_SetRenderVSync(_renderer, -1))
        {
            DGEX_CORE_ERROR("VSync not supported: {0}", SDL_GetError());
        }
    }
}

void SetWindowPropertiesHint(const WindowProperties& properties)
{
    sWindowPropertiesHint = properties;
}

Scope<Window> CreateWindow()
{
    Expected<Ptr<Window>, dgex_error_t> expected = Window::Create(sWindowPropertiesHint);
    if (!expected)
    {
        DGEX_CORE_CRITICAL("Failed to create window: {0}", expected.Error());
        return nullptr;
    }
    return Scope<Window>(expected.Value());
}

DGEX_END
