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

#include "Impl/MainLoop.h"

#include "DgeX/Device/Graphics/Graphics.h"
#include "DgeX/Device/Graphics/Renderer.h"
#include "DgeX/Device/Graphics/Window.h"
#include "DgeX/Error.h"
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

// OnUpdate implementation.
static bool OnUpdate()
{
    if (int r = sOnUpdate(sAppContext); r != 0)
    {
        DGEX_CORE_WARN("DgeXOnUpdate: {0}", r);
        return true;
    }
    return false;
}

// OnEvent implementation.
static void OnEvent()
{
    sOnEvent(sAppContext);
}

int DgeXMainImplWithCallbacks(CommandLineArgs args, DgeXOnInitEntry onInit, DgeXOnStartEntry onStart,
                              DgeXOnUpdateEntry onUpdate, DgeXOnEventEntry onEvent, DgeXOnExitEntry onExit)
{
    Preamble();

    // ==============================================================
    // Initialization
    // --------------------------------------------------------------

    if (int r = onInit(args, &sAppContext); r != 0)
    {
        DGEX_CORE_ERROR("OnInit error: {0}", r);
        return DGEX_ERROR_CUSTOM_INIT;
    }

    if (dgex_error_t r = InitGraphics(); r != DGEX_SUCCESS)
    {
        DGEX_CORE_CRITICAL("Failed to initialize graphics device: {0}", r);
    }

    if (int r = onStart(sAppContext); r != 0)
    {
        DGEX_CORE_ERROR("OnStart error: {0}", r);
        return DGEX_ERROR_CUSTOM_START;
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

    DestroyGraphics();

    return 0;
}
