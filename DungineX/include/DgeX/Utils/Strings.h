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
 *                    Last Update : June 8, 2025                              *
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

bool StartsWith(const std::string& source, const std::string& pattern);
bool EndsWith(const std::string& source, const std::string& pattern);

} // namespace Strings

DGEX_END
