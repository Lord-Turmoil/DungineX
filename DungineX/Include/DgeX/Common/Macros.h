#pragma once

#define DUNGEON_ENGINE_X

// Define the DungineX version
#define DGEX_VERSION_MAJOR 0
#define DGEX_VERSION_MINOR 1
#define DGEX_VERSION_PATCH 0

// Define the DungineX debug flag
#if defined(DEBUG) || defined(_DEBUG)
#define DGEX_DEBUG
#endif

#ifdef _RELEASE
#define DGEX_RELEASE
#endif

#ifdef _PUBLISH
#define DGEX_PUBLISH
#endif

// Enable assertions only in debug and release builds
#if defined(DGEX_DEBUG) || defined(DGEX_RELEASE)
#define DGEX_ENABLE_ASSERT
#endif

// Only do logging with this macro defined
#define DGEX_ENABLE_LOGGING

// Whether log trace messages
// #define DGEX_VERBOSE_LOGGING

// Attach console to the application for non-publishing builds
#ifndef DGEX_PUBLISH
#define DGEX_ATTACH_CONSOLE
#endif

#define DGEX_BEGIN                                                                                                     \
    namespace DgeX                                                                                                     \
    {
#define DGEX_END }

// Define backend platform
#define DGEX_OPENGL 1

#define _DGEX_CORE_
