#pragma once

#include "DgeX/Application/Application.h"
#include "DgeX/Core/Log.h"
#include "DgeX/Version.h"

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
 */
inline int main(int argc, char* argv[])
{
    using namespace DgeX;

#ifdef DGEX_ATTACH_CONSOLE
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
#endif

    Log::Init("LogConfig.xml");

    Log::GetCoreLogger()->Error("DgeX Engine {0}", DGEX_VERSION_STRING);
    Log::GetCoreLogger()->Error("Copyright (C) 2024 New Desire Studios");

    Application* app;
    DGEX_TIME_BEGIN("Initialize application");
    app = CreateApplication({ argc, argv });
    DGEX_TIME_END();

    DGEX_LOG_INFO("Run application");
    app->Run();
    DGEX_LOG_INFO("Application stopped");

    DGEX_TIME_BEGIN("Shutting down DungineX");
    delete app;
    DGEX_TIME_END();

    DGEX_LOG_INFO("See you next time!");

#ifdef DGEX_ATTACH_CONSOLE
    FreeConsole();
#endif

    return 0;
}
