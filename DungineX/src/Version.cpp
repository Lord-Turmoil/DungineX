/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Version.cpp                               *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : May 26, 2025                              *
 *                                                                            *
 *                    Last Update : May 26, 2025                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Project version information.                                               *
 ******************************************************************************/

#include "DgeX/Version.h"

DGEX_BEGIN

std::string GetDgeXVersion()
{
    return { DGEX_VERSION_STRING };
}

void GetDgeXVersion(int* major, int* minor, int* patch)
{
    if (major)
    {
        *major = DGEX_VERSION_MAJOR;
    }

    if (minor)
    {
        *minor = DGEX_VERSION_MINOR;
    }

    if (patch)
    {
        *patch = DGEX_VERSION_PATCH;
    }
}

DGEX_END
