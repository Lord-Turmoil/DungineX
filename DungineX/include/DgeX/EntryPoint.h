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

#ifndef _DGEX_ENTRYPOINT_H_
#define _DGEX_ENTRYPOINT_H_

#include "Application/Event/Event.h"
#include "DgeX/Defines.h"
#include "Utils/Types.h"

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

using DgeXMainEntry = int (*)(const CommandLineArgs&);

using DgeXOnInitEntry = int (*)(const CommandLineArgs&, void**);
using DgeXOnStartEntry = int (*)(void*);
using DgeXOnUpdateEntry = int (*)(void*);
using DgeXOnEventEntry = int (*)(void*, DgeX::Ref<DgeX::Event>);
using DgeXOnExitEntry = int (*)(void*);

/**
 * A unified structure for all callbacks.
 */
struct DgeXCallbackRegistration
{
    DgeXOnInitEntry OnInit = nullptr;
    DgeXOnStartEntry OnStart = nullptr;
    DgeXOnUpdateEntry OnUpdate = nullptr;
    DgeXOnEventEntry OnEvent = nullptr;
    DgeXOnExitEntry OnExit = nullptr;

    DGEX_API DgeXCallbackRegistration() = default;

    DGEX_API DgeXCallbackRegistration(DgeXOnInitEntry onInit, DgeXOnStartEntry onStart, DgeXOnUpdateEntry onUpdate,
                                      DgeXOnEventEntry onEvent, DgeXOnExitEntry onExit)
        : OnInit(onInit), OnStart(onStart), OnUpdate(onUpdate), OnEvent(onEvent), OnExit(onExit)
    {
    }

    DGEX_API DgeXCallbackRegistration(DgeXOnInitEntry onInit, DgeXOnStartEntry onStart, DgeXOnExitEntry onExit)
        : OnInit(onInit), OnStart(onStart), OnExit(onExit)
    {
    }
};

/**
 * There are 5 callbacks to be defined.
 *
 * - OnInit
 * - OnStart
 * - OnUpdate
 * - OnEvent
 * - OnExit
 */
#ifdef DGEX_USE_CALLBACKS

/**
 * @brief Callback before the engine initialization.
 *
 * This is called before the engine initializes, when nothing is ready.
 * You can do some preparations to set hint for the initialization of
 * the engine.
 *
 * @param args Commandline arguments.
 * @param context Custom application context.
 * @return Whether initialization succeeded or not.
 *         0 for success, others as error code.
 */
extern int DgeXOnInit(const CommandLineArgs& args, void** context);

/**
 * @brief Callback after the engine initialization, before the main loop.
 *
 * All devices are initialized before invoking this callback, so you can
 * prepare device-dependent resources.
 *
 * @param context Custom application context.
 * @return Whether initialization succeeded or not.
 *         0 for success, others as error code.
 */
extern int DgeXOnStart(void* context);

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
extern int DgeXOnEvent(void* context, DgeX::Ref<DgeX::Event> event);

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
DGEX_API int DgeXMainImplWithCallbacks(CommandLineArgs args, const DgeXCallbackRegistration& callbacks);

#ifndef DGEX_ENTRYPOINT_IMPL
#include "DgeX/Impl/EntryPointImpl.h"
#endif

// ============================================================================
// Callback Aliases
// ----------------------------------------------------------------------------

#ifdef DGEX_USE_CALLBACKS

#define OnInit   DgeXOnInit
#define OnStart  DgeXOnStart
#define OnUpdate DgeXOnUpdate
#define OnEvent  DgeXOnEvent
#define OnExit   DgeXOnExit

#else

#define main DgeXMain

#endif // DGEX_USE_CALLBACKS

#endif
