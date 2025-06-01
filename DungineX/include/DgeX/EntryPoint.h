/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : EntryPoint.h                              *
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
 * Entry point of the game.                                                   *
 * -------------------------------------------------------------------------- *
 * NOTE:                                                                      *
 *                                                                            *
 * This file should only be included in one source file.                      *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

/**
 * @brief A simple wrapper for command line arguments.
 */
struct CommandLineArgs
{
    int Count = 0;
    char** Args = nullptr;

    DGEX_API const char* operator[](int index) const
    {
        return Args[index];
    }
};

typedef int (*OnInitEntry)(const CommandLineArgs&, void**);
typedef int (*OnExitEntry)(void*, int);
typedef int (*OnUpdateEntry)(void*);
typedef int (*OnEventEntry)(void*);

/**
 * @brief Main entry point for the game built with DungineX.
 *
 * This entry should be defined in the client game code, and can't be
 * marked with DGEX_API. As the .dll target won't load it.
 *
 * @param args Commandline arguments.
 * @param context Custom application context.
 * @return Whether initialization succeeded or not.
 *         0 for success, others as error code.
 */
extern int DgeXOnInit(const CommandLineArgs& args, void** context);

/**
 * @brief This is called when the game's main loop ends.
 *
 * @param context Custom application context set in DgeXInit.
 * @param result  Return value from DgeXOnUpdate.
 * @return Whether the cleanup succeeded or not.
 *         0 for success, others as error code.
 */
extern int DgeXOnExit(void* context, int result);

/**
 * @brief Default implementation of DgeXOnUpdate.
 *
 * @return Whether update succeeded or not.
 *         0 for success, others as error code.
 */
int DGEX_API DgeXOnUpdateDefault();

/**
 * @brief Default implementation of DgeXOnEvent.
 *
 * @return Whether event handling succeeded or not.
 *         0 for success, others as error code.
 */
int DGEX_API DgeXOnEventDefault();

#ifdef DGEX_CUSTOM_LOOP

/**
 * @brief This is called every frame of the game.
 *
 * @param context Custom application context set in DgeXInit.
 * @return Whether the game should continue or not.
 *         0 for continue, others to exit.
 */
extern int DgeXOnUpdate(void* context);

/**
 * @brief This is called when the game receives an event.
 *
 * @param context Custom application context set in DgeXInit.
 * @return Whether the event handling succeeded or not.
 *         0 for continue, others as error code.
 */
extern int DgeXOnEvent(void* context);

#endif // DGEX_CUSTOM_LOOP

// ============================================================================
// Main Entry
// ----------------------------------------------------------------------------

#include <SDL3/SDL.h>

// ============================================================================
// Native Implementations
// ----------------------------------------------------------------------------

DGEX_API SDL_AppResult AppInitImpl(void** appstate, CommandLineArgs& args, OnInitEntry entry);

DGEX_API SDL_AppResult AppEventImpl(void* appstate, SDL_Event* event, OnEventEntry entry);
DGEX_API SDL_AppResult AppEventNative(void* appstate, SDL_Event* event);

DGEX_API SDL_AppResult AppIterateImpl(void* appstate, OnUpdateEntry entry);
DGEX_API SDL_AppResult AppIterateNative(void* appstate);

DGEX_API void AppQuitImpl(void* appstate, SDL_AppResult result, OnExitEntry entry);

// ============================================================================
// Main Entry
// ----------------------------------------------------------------------------

#ifndef DGEX_ENTRYPOINT_IMPL

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

inline SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    CommandLineArgs args{ argc, argv };
    return AppInitImpl(appstate, args, DgeXOnInit);
}

inline SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
#ifdef DGEX_CUSTOM_LOOP
    return AppEventImpl(appstate, event, DgeXOnEvent);
#else
    return AppEventNative(appstate, event);
#endif
}

inline SDL_AppResult SDL_AppIterate(void* appstate)
{
#ifdef DGEX_CUSTOM_LOOP
    return AppIterateImpl(appstate, DgeXOnUpdate);
#else
    return AppIterateNative(appstate);
#endif
}

inline void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    AppQuitImpl(appstate, result, DgeXOnExit);
}

#endif // DGEX_ENTRYPOINT_IMPL
