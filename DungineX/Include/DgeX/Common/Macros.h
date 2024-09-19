#pragma once

#define DUNGEON_ENGINE_X

// define this to publish the application
// #define DGEX_PUBLISH

// Define the DungineX version
#define DGEX_VERSION_MAJOR 0
#define DGEX_VERSION_MINOR 1
#define DGEX_VERSION_PATCH 0

// Define the DungineX debug flag
#if defined(DEBUG) || defined(_DEBUG)
#define DGEX_DEBUG
#else
#define DGEX_RELEASE
#endif

// Enable assertions only in debug and release builds
#if defined(DGEX_DEBUG)
#define DGEX_ENABLE_ASSERT
#endif

// Only do logging with this macro defined
#define DGEX_ENABLE_LOGGING

// Whether log trace messages
// #define DGEX_VERBOSE_LOGGING

// Attach console to the application for non-publishing builds
#ifndef DGEX_RELEASE
#define DGEX_ATTACH_CONSOLE
#endif

// Always show splash screen
// #define DGEX_ALWAYS_SPLASH

#define DGEX_BEGIN                                                                                                     \
    namespace DgeX                                                                                                     \
    {
#define DGEX_END }

#define DGEX ::DgeX::

// Define backend platform
#define DGEX_OPENGL

#define _DGEX_CORE_
