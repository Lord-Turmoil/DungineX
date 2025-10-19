/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Defines.h                                 *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : May 25, 2025                              *
 *                                                                            *
 *                    Last Update : June 19, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Global definitions.                                                        *
 ******************************************************************************/

#pragma once

// ============================================================================
// Build Configuration
// ----------------------------------------------------------------------------

#if defined(DEBUG) || defined(_DEBUG)
#define DGEX_DEBUG // Debug build
#else
#define DGEX_RELEASE // Release build
#endif

#if 0 // Should be set with CMake options.

// Exporting the DungineX library as a DLL.
#define DGEX_EXPORT

// Adding extra features to kindly remind users of this project.
#define DGEX_PUBLISH

// Enable assertions.
#define DGEX_ENABLE_ASSERT

// Use high precision time representation.
#define DGEX_HIGH_PRECISION_TIME

#endif

// ============================================================================
// Platform Detection
// ----------------------------------------------------------------------------

#if (defined(_WIN32) || defined(_WIN64))
#define DGEX_PLATFORM_WINDOWS
#elif (defined(__linux__))
#define DGEX_PLATFORM_LINUX
#elif (defined(__APPLE__) || defined(__MACH__))
#define DGEX_PLATFORM_MACOS
#else
#error "Unsupported platform"
#endif

// clang-format off
#ifdef DGEX_PLATFORM_WINDOWS

#ifdef DGEX_EXPORT
#   ifdef DGEX_ENGINE
#       define DGEX_API  __declspec(dllexport)
#       define DGEX_DATA __declspec(dllexport)
#   else
#       define DGEX_API
#       define DGEX_DATA __declspec(dllimport)
#   endif // DGEX_ENGINE
#else
#   define DGEX_API
#   define DGEX_DATA
#endif

#else

#ifdef DGEX_EXPORT
#   ifdef DGEX_ENGINE
#       define DGEX_API  __attribute__((visibility("default")))
#       define DGEX_DATA __attribute__((visibility("default")))
#   else
#       define DGEX_API
#       define DGEX_DATA
#   endif // DGEX_ENGINE
#else
#   define DGEX_API
#   define DGEX_DATA
#endif

#endif // DGEX_PLATFORM_WINDOWS
// clang-format on

#ifdef __cplusplus

#define DGEX_EXTERN_C extern "C"
#define DGEX_EXTERN_C_BEGIN                                                                                            \
    extern "C"                                                                                                         \
    {
#define DGEX_EXTERN_C_END }

#else

#define DGEX_EXTERN_C
#define DGEX_EXTERN_C_BEGIN
#define DGEX_EXTERN_C_END

#endif // __cplusplus

// ============================================================================
// Global Namespace & Macros
// ----------------------------------------------------------------------------

#define DGEX_BEGIN                                                                                                     \
    namespace DgeX                                                                                                     \
    {
#define DGEX_END }

#define DGEX ::DgeX::

#define DUNGINEX       "DungineX"
#define DUNGINEX_SHORT "DgeX"
