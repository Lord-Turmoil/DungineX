#pragma once

#include "DgeX/Application/Application.h"

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
    DgeX::Log::Init(DgeX::LogLevel::All);
#else
    Log::Init(LogLevel::Debug);
#endif
#else
#ifdef DGEX_PUBLISH
    DgeX::Log::Init(DgeX::LogLevel::Warning);
#else
    DgeX::Log::Init(DgeX::LogLevel::Info);
#endif
#endif

    DGEX_CORE_ERROR("DgeX Engine {0}.{1}.{2}", DGEX_VERSION_MAJOR, DGEX_VERSION_MINOR, DGEX_VERSION_PATCH);
    DGEX_CORE_ERROR("Copyright (C) 2024 New Desire Studios");

    DGEX_LOG_INFO("Initializing application");
    const auto app = CreateApplication({ argc, argv });
    DGEX_LOG_INFO("Application initialized");

    DGEX_LOG_INFO("Running application");
    app->Run();
    DGEX_LOG_INFO("Application stopped");

    DGEX_CORE_INFO("Shutting down DgeX Engine");
    delete app;
    DGEX_LOG_INFO("See you next time!");

#ifdef DGEX_ATTACH_CONSOLE
    FreeConsole();
#endif

    return 0;
}
