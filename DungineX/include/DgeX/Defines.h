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

// ============================================================================
// Build Configuration
// ----------------------------------------------------------------------------

#if defined(DEBUG) || defined(_DEBUG)
#define DGEX_DEBUG // Debug build
#else
#define DGEX_RELEASE // Release build
#endif

#if 0 // Should be set with CMake options.

// Adding extra features to kindly remind users of this project.
#define DGEX_PUBLISH
// Enable assertions.
#define DGEX_ENABLE_ASSERT

#endif

// ============================================================================
// Global Namespace     
// ----------------------------------------------------------------------------

#define DGEX_BEGIN                                                                                                     \
    namespace DgeX                                                                                                     \
    {
#define DGEX_END }

#define DGEX ::DgeX::
