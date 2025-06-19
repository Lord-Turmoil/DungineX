/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Strings.h                                 *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 8, 2025                              *
 *                                                                            *
 *                    Last Update : June 19, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * String utility functions.                                                  *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <string>

DGEX_BEGIN

namespace Strings
{

/**
 * @brief Check if a string starts with a given prefix.
 *
 * @param source Source string.
 * @param pattern Prefix pattern.
 * @return Whether the source string starts with the pattern or not.
 */
bool StartsWith(const std::string& source, const std::string& pattern);

/**
 * @brief Check if a string ends with a given suffix.
 *
 * @param source Source string.
 * @param pattern Suffix pattern.
 * @return Whether the source string ends with the pattern or not.
 */
bool EndsWith(const std::string& source, const std::string& pattern);

} // namespace Strings

DGEX_END
