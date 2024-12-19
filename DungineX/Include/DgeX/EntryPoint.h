#pragma once

#include "DgeX/Application/Application.h"
#include "DgeX/Common/Log.h"

// Bad Windows.h... :(
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#ifdef interface
#undef interface
#endif

/**
 * @brief Entry point of DungineX application.
 * @paragraph For Visual Studio user, to disable
 */
int main(int argc, char* argv[])
{
#ifdef DGEX_ATTACH_CONSOLE
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
#endif

#ifdef DGEX_DEBUG
#ifdef DGEX_VERBOSE_LOGGING
    DGEX Log::Init(DGEX LogLevel::All);
#else
    DGEX Log::Init(DGEX LogLevel::Debug);
#endif
#else
#ifdef DGEX_PUBLISH
    DGEX Log::Init(DGEX LogLevel::Warning);
#else
    DGEX Log::Init(DGEX LogLevel::Info);
#endif
#endif

    DGEX_CORE_ERROR("DgeX Engine {0}.{1}.{2}", DGEX_VERSION_MAJOR, DGEX_VERSION_MINOR, DGEX_VERSION_PATCH);
    DGEX_CORE_ERROR("Copyright (C) 2024 New Desire Studios");

    DgeX::Application* app;
    DGEX_TIME_BEGIN("Initialize application");
    app = DGEX CreateApplication({ argc, argv });
    DGEX_TIME_END();

    DGEX_LOG_INFO("Running application");
    app->Run();
    DGEX_LOG_INFO("Application stopped");

    DGEX_TIME_BEGIN("Shutting down DgeX Engine");
    delete app;
    DGEX_TIME_END();

    DGEX_LOG_INFO("See you next time!");

#ifdef DGEX_ATTACH_CONSOLE
    FreeConsole();
#endif

    return 0;
}
