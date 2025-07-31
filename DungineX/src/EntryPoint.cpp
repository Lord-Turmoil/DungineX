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
#include "DgeX/Utils/Assert.h"
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
static bool OnUpdateImpl()
{
    if (int r = sOnUpdate(sAppContext); r != 0)
    {
        DGEX_CORE_WARN("DgeXOnUpdate: {0}", r);
        return true;
    }
    return false;
}

// OnEvent implementation.
static void OnEventImpl(Ref<Event> event)
{
    sOnEvent(sAppContext, event);
}

int DgeXMainImplWithCallbacks(CommandLineArgs args, const DgeXCallbackRegistration& callbacks)
{
    Preamble();

    DGEX_ASSERT(callbacks.OnInit, "OnInit callback must be provided.");
    DGEX_ASSERT(callbacks.OnStart, "OnStart callback must be provided.");
    DGEX_ASSERT(callbacks.OnExit, "OnExit callback must be provided.");

    // ==============================================================
    // Initialization
    // --------------------------------------------------------------

    if (int r = callbacks.OnInit(args, &sAppContext); r != 0)
    {
        DGEX_CORE_ERROR("OnInit error: {0}", r);
        return DGEX_ERROR_CUSTOM_INIT;
    }

    if (dgex_error_t r = InitGraphics(); r != DGEX_SUCCESS)
    {
        DGEX_CORE_CRITICAL("Failed to initialize graphics device: {0}", r);
    }

    if (int r = callbacks.OnStart(sAppContext); r != 0)
    {
        DGEX_CORE_ERROR("OnStart error: {0}", r);
        return DGEX_ERROR_CUSTOM_START;
    }

    // ==============================================================
    // Main Loop
    // --------------------------------------------------------------

    sOnUpdate = callbacks.OnUpdate;
    sOnEvent = callbacks.OnEvent;
    MainLoop(OnUpdateImpl, OnEventImpl);

    // ==============================================================
    // Clean Up
    // --------------------------------------------------------------

    if (int r = callbacks.OnExit(sAppContext); r != 0)
    {
        DGEX_CORE_ERROR("DgeXOnExit failed: {0}", r);
    }

    DestroyGraphics();

    return 0;
}
