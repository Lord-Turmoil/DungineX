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
#error "This file should only be included directly, but only in DgeX/EntryPoint.h."
#endif // _DGEX_ENTRYPOINT_H_

#include "DgeX/EntryPoint.h" // still including this for intellisense and documentation

// ============================================================================
// Default Callback Implementations
// ----------------------------------------------------------------------------

#ifdef DGEX_USE_DEFAULT_ONINIT

int OnInit(const CommandLineArgs& args, void** context)
{
    DGEX_CORE_DEBUG("Default OnInit");

    SetWindowPropertiesHint({ "Hello There", 640, 480, DgexWindowResizable });

    *context = nullptr;

    return DGEX_SUCCESS;
}

#endif // DGEX_USE_DEFAULT_ONINIT

#ifdef DGEX_USE_DEFAULT_ONSTART

int OnStart(void* context)
{
    return DGEX_SUCCESS;
}

#endif // DGEX_USE_DEFAULT_ONSTART

#ifdef DGEX_USE_DEFAULT_ONUPDATE

int OnUpdate(void* context)
{
    // Default implementation does nothing.
    return DGEX_SUCCESS;
}

#endif // DGEX_USE_DEFAULT_ONUPDATE

#ifdef DGEX_USE_DEFAULT_ONEVENT

int OnEvent(void* context)
{
    // Default implementation does nothing.
    return DGEX_SUCCESS;
}

#endif // DGEX_USE_DEFAULT_ONEVENT

#ifdef DGEX_USE_DEFAULT_ONEXIT

int OnExit(void* context)
{
    DGEX_CORE_DEBUG("Default OnExit");
    return DGEX_SUCCESS;
}

#endif // DGEX_USE_DEFAULT_ONEXIT

inline int main(int argc, char* argv[])
{
#ifdef DGEX_USE_CALLBACKS
    auto callbacks = DgeXCallbackRegistration(DgeXOnInit, DgeXOnStart, DgeXOnUpdate, DgeXOnEvent, DgeXOnExit);
    return DgeXMainImplWithCallbacks({ argc, argv }, callbacks);
#else
    return DgeXMainImpl({ argc, argv }, DgeXMain);
#endif
}
