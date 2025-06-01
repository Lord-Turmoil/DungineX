/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : EntryPoint.cpp                            *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : May 29, 2025                              *
 *                                                                            *
 *                    Last Update : June 1, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Entry point implementation.                                                *
 ******************************************************************************/

// Prevent inclusion of `main` function.
#define DGEX_ENTRYPOINT_IMPL
#include "DgeX/EntryPoint.h"

#include "DgeX/Utils/Log.h"
#include "DgeX/Version.h"

using namespace DgeX;

SDL_AppResult AppInitImpl(void** appstate, CommandLineArgs& args, OnInitEntry entry)
{
    Log::Init();

    DGEX_CORE_CRITICAL("Made with DungineX " DGEX_VERSION_STRING);
    DGEX_CORE_CRITICAL("Copyright (C) New Desire Studios " DGEX_YEAR_STRING);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        DGEX_CORE_CRITICAL("Failed to initialize SDL3: {0}", ERROR_GRAPHICS(1));
        return SDL_APP_FAILURE;
    }

    if (int r = entry(args, appstate); r != 0)
    {
        DGEX_CORE_ERROR("DgeXOnInit error: {0}", r);
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult AppEventImpl(void* appstate, SDL_Event* event, OnEventEntry entry)
{
    // TODO:
    // Implement internal event handling.

    if (int r = entry(appstate); r != 0)
    {
        DGEX_CORE_ERROR("DgeXOnEvent error: {0}", r);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult AppEventNative(void* appstate, SDL_Event* event)
{
    // TODO:
    // Implement event dispatching.

    return SDL_APP_CONTINUE;
}

SDL_AppResult AppIterateImpl(void* appstate, OnUpdateEntry entry)
{
    if (int r = entry(appstate); r != 0)
    {
        DGEX_CORE_WARN("DgeXOnUpdate: {0}", r);
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult AppIterateNative(void* appstate)
{
    // TODO:
    // Implement game loop.

    return SDL_APP_CONTINUE;
}

void AppQuitImpl(void* appstate, SDL_AppResult result, OnExitEntry entry)
{
    if (int r = entry(appstate, result); r != 0)
    {
        DGEX_CORE_ERROR("DgeXOnExit failed: {0}", r);
    }
}
