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

typedef int (*DgeXMainEntry)(const CommandLineArgs&);

typedef int (*DgeXOnInitEntry)(const CommandLineArgs&, void**);
typedef int (*DgeXOnUpdateEntry)(void*);
typedef int (*DgeXOnEventEntry)(void*);
typedef int (*DgeXOnExitEntry)(void*);

#ifdef DGEX_USE_CALLBACKS

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

/**
 * @brief This is called when the game's main loop ends.
 *
 * @param context Custom application context set in DgeXInit.
 * @param result  Return value from DgeXOnUpdate.
 * @return Whether the cleanup succeeded or not.
 *         0 for success, others as error code.
 */
extern int DgeXOnExit(void* context);

#else

extern int DgeXMain(const CommandLineArgs& args);

#endif // DGEX_USE_CALLBACKS

// ============================================================================
// Main Entry
// ----------------------------------------------------------------------------

DGEX_API int DgeXMainImpl(CommandLineArgs args, DgeXMainEntry entry);
DGEX_API int DgeXMainImplWithCallbacks(CommandLineArgs args, DgeXOnInitEntry onInit, DgeXOnUpdateEntry onUpdate,
                                       DgeXOnEventEntry onEvent, DgeXOnExitEntry onExit);

// ============================================================================
// Main Entry
// ----------------------------------------------------------------------------

#ifndef DGEX_ENTRYPOINT_IMPL

int main(int argc, char* argv[])
{
#ifdef DGEX_USE_CALLBACKS
    return DgeXMainImplWithCallbacks({ argc, argv }, DgeXOnInit, DgeXOnUpdate, DgeXOnEvent, DgeXOnExit);
#else
    return DgeXMainImpl({ argc, argv }, DgeXMain);
#endif
}

#ifndef DGEX_USE_CALLBACKS
#define main DgeXMain
#endif

#endif // DGEX_ENTRYPOINT_IMPL
