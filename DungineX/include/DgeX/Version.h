/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Version.h                                 *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : May 25, 2025                              *
 *                                                                            *
 *                    Last Update : May 25, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Project version information.                                               *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Utils/Macros.h"

#include <string>

#define DGEX_VERSION_MAJOR 0
#define DGEX_VERSION_MINOR 1
#define DGEX_VERSION_PATCH 0

// Revision is only used for file property.
#define DGEX_VERSION_REVISION 0

#define DGEX_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
#define DGEX_VERSION                         DGEX_TO_VERSION(DGEX_VERSION_MAJOR, DGEX_VERSION_MINOR, DGEX_VERSION_PATCH)

// clang-format off
#define DGEX_VERSION_STRING                                                                                            \
    DGEX_STRINGIFY(DGEX_VERSION_MAJOR)                                                                                 \
    "."                                                                                                                \
    DGEX_STRINGIFY(DGEX_VERSION_MINOR)                                                                                 \
    "."                                                                                                                \
    DGEX_STRINGIFY(DGEX_VERSION_PATCH)
// clang-format on

DGEX_BEGIN

/**
 * @brief Get the DungineX version as a string.
 *
 * @return std::string The version string in the format "major.minor.patch".
 */
DGEX_API std::string GetDgeXVersion();

/**
 * @brief Get the DungineX versions as individual numbers.
 *
 * If you're not interested in all three version numbers, you can pass `nullptr`
 * for the ones you don't need.
 *
 * @param major Returns the major version number.
 * @param minor Returns the minor version number.
 * @param patch Returns the patch version number.
 */
DGEX_API void GetDgeXVersion(int* major, int* minor, int* patch);

DGEX_END
