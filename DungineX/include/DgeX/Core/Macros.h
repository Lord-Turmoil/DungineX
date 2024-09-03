#pragma once

#define DUNGEON_ENGINE_X
#define _DGEX_CORE_

// Check if the platform is Windows
#if !(defined(_WIN32) || defined(_WIN64))
#error "DungineX only supports Windows platform!"
#endif

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
#ifndef DGEX_PUBLISH
#define DGEX_ATTACH_CONSOLE
#endif

// Always show splash screen for publishing builds
#ifdef DGEX_PUBLISH
#define DGEX_ALWAYS_SPLASH
#endif

#define DGEX_EXPAND_MACRO(x)    x
#define DGEX_STRINGIFY_MACRO(x) #x
#define DGEX_STRINGIFY(x)       DGEX_STRINGIFY_MACRO(x)

#define DGEX_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define DGEX_VERSION                         DGEX_TO_VERSION(DGEX_VERSION_MAJOR, DGEX_VERSION_MINOR, DGEX_VERSION_PATCH)

#define BIT(x) (1 << (x))

#define DGEX_BIND_EVENT_FN(fn)                                                                                         \
    [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define DGEX_BEGIN                                                                                                     \
    namespace DgeX                                                                                                     \
    {
#define DGEX_END }

#define DGEX ::DgeX::
