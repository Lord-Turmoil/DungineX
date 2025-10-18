/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Strings.h                                 *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : June 8, 2025                              *
 *                                                                            *
 *                    Last Update : October 18, 2025                          *
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

bool Equals(const char* str1, const char* str2);

/**
 * @brief Check if a string starts with a given prefix.
 *
 * @param source Source string.
 * @param pattern Prefix pattern.
 * @return Whether the source string starts with the pattern or not.
 */
bool StartsWith(const std::string& source, const std::string& pattern);

/**
 * @brief Check if a C-style string starts with a given prefix.
 *
 * @param source Source C-style string.
 * @param pattern Prefix pattern.
 * @return Whether the source string starts with the pattern or not.
 */
bool StartsWith(const char* source, const char* pattern);

template <typename... Args>
bool StartsWith(const char* source, const char* pattern, Args... args)
{
    return StartsWith(source, pattern) || StartsWith(source, args...);
}

/**
 * @brief Check if a string ends with a given suffix.
 *
 * @param source Source string.
 * @param pattern Suffix pattern.
 * @return Whether the source string ends with the pattern or not.
 */
bool EndsWith(const std::string& source, const std::string& pattern);

template <typename... Args>
bool EndsWith(const std::string& source, const std::string& pattern, Args... args)
{
    return EndsWith(source, pattern) || EndsWith(source, args...);
}

/**
 * @brief Check if a C-style string ends with a given suffix.
 *
 * @param source Source C-style string.
 * @param pattern Suffix pattern.
 * @return Whether the source string ends with the pattern or not.
 */
bool EndsWith(const char* source, const char* pattern);

} // namespace Strings

DGEX_END
