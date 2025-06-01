/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Defines.h                                 *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : May 25, 2025                              *
 *                                                                            *
 *                    Last Update : May 26, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Global definitions.                                                        *
 ******************************************************************************/

#pragma once

// ============================================================================
// Platform Detection
// ----------------------------------------------------------------------------

#if !(defined(_WIN32) || defined(_WIN64))
#error "DungineX only supports Windows platform!"
#endif

#ifdef DGEX_EXPORT
#define DGEX_API __declspec(dllexport)
#else
#define DGEX_API
#endif

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

#endif

// ============================================================================
// Build Configuration
// ----------------------------------------------------------------------------

#if defined(DEBUG) || defined(_DEBUG)
#define DGEX_DEBUG // Debug build
#else
#define DGEX_RELEASE // Release build
#endif

#if 0 // Should be set with CMake options.

// Mark that we're in the engine, not client.
#define DGEX_ENGINE
// Adding extra features to kindly remind users of this project.
#define DGEX_PUBLISH
// Enable assertions.
#define DGEX_ENABLE_ASSERT

#endif

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
