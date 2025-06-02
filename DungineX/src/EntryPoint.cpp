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

#include "DgeX/Device/Graphics/Window.h"
#include "DgeX/Error.h"
#include "DgeX/Impl/MainLoop.h"
#include "DgeX/Utils/Log.h"
#include "DgeX/Version.h"

using namespace DgeX;

static void* sAppContext = nullptr; // client app context

static DgeXOnEventEntry sOnEvent;
static DgeXOnUpdateEntry sOnUpdate;

static void Preamble()
{
    Log::Init();

    DGEX_CORE_CRITICAL("Made with DungineX " DGEX_VERSION_STRING);
    DGEX_CORE_CRITICAL("Copyright (C) New Desire Studios " DGEX_YEAR_STRING);
}

int DgeXMainImpl(CommandLineArgs args, DgeXMainEntry entry)
{
    Preamble();

    return entry(args);
}

static bool OnUpdate()
{
    if (int r = sOnUpdate(sAppContext); r != 0)
    {
        DGEX_CORE_WARN("DgeXOnUpdate: {0}", r);
        return true;
    }
    return false;
}

static void OnEvent()
{
    sOnEvent(sAppContext);
}

int DgeXMainImplWithCallbacks(CommandLineArgs args, DgeXOnInitEntry onInit, DgeXOnUpdateEntry onUpdate,
                              DgeXOnEventEntry onEvent, DgeXOnExitEntry onExit)
{
    Preamble();

    // ==============================================================
    // Initialization
    // --------------------------------------------------------------

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        DGEX_CORE_ERROR(SDL_GetError());
        DGEX_CORE_CRITICAL("Failed to initialize SDL3: {0}", DGEX_ERROR_SDL_INIT);
        return DGEX_ERROR_SDL_INIT;
    }

    if (int r = onInit(args, &sAppContext); r != 0)
    {
        DGEX_CORE_ERROR("DgeXOnInit error: {0}", r);
        return DGEX_ERROR_CUSTOM_INIT;
    }

    Ref<Window> window = CreateWindow();
    if (!window)
    {
        DGEX_CORE_CRITICAL("Failed to initialize window");
        return DGEX_ERROR_WINDOW_INIT;
    }

    // ==============================================================
    // Main Loop
    // --------------------------------------------------------------
    sOnUpdate = onUpdate;
    sOnEvent = onEvent;
    MainLoop(OnUpdate, OnEvent);

    // ==============================================================
    // Clean Up
    // --------------------------------------------------------------
    if (int r = onExit(sAppContext); r != 0)
    {
        DGEX_CORE_ERROR("DgeXOnExit failed: {0}", r);
    }

    SDL_Quit();

    return 0;
}
