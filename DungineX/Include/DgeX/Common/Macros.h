#pragma once

#define DUNGEON_ENGINE_X

// Define the DungineX version
#define DGEX_VERSION_MAJOR 0
#define DGEX_VERSION_MINOR 1
#define DGEX_VERSION_PATCH 0

#define DGEX_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define DGEX_VERSION                         DGEX_TO_VERSION(DGEX_VERSION_MAJOR, DGEX_VERSION_MINOR, DGEX_VERSION_PATCH)

// Define the DungineX debug flag
#if defined(DEBUG) || defined(_DEBUG)
#define DGEX_DEBUG
#else
#define DGEX_RELEASE
#endif

#if defined(PUBLISH) || defined(_PUBLISH)
#define DGEX_PUBLISH
#endif

// Only do logging with this macro defined
#define DGEX_ENABLE_LOGGING
#define DGEX_ENABLE_TIMING

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

#define _DGEX_CORE_
