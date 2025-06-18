/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Strings.cpp                               *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : June 8, 2025                              *
 *                                                                            *
 *                    Last Update : June 8, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * String utility functions.                                                  *
 ******************************************************************************/

#include "DgeX/Utils/Strings.h"

DGEX_BEGIN

bool Strings::StartsWith(const std::string& source, const std::string& pattern)
{
    size_t sourceLength = source.length();
    size_t patternLen = pattern.length();

    if (patternLen > sourceLength)
    {
        return false;
    }

    for (size_t i = 0; i < patternLen; i++)
    {
        if (source[i] != pattern[i])
        {
            return false;
        }
    }

    return true;
}

bool Strings::EndsWith(const std::string& source, const std::string& pattern)
{
    size_t sourceLength = source.length();
    size_t patternLen = pattern.length();

    if (sourceLength < patternLen)
    {
        return false;
    }

    for (size_t i = 0; i < patternLen; i++)
    {
        if (source[sourceLength - i] != pattern[patternLen - i])
        {
            return false;
        }
    }

    return true;
}

DGEX_END
