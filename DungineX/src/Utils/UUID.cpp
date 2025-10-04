/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : UUID.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 4, 2025                           *
 *                                                                            *
 *                    Last Update : October 4, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * UUID implementation.                                                       *
 ******************************************************************************/

#include <sstream>

#include "DgeX/Utils/UUID.h"
#include "DgeX/Utils/Random.h"

DGEX_BEGIN

UUID::UUID() : _uuid(Utils::Random::RandomUInt64())
{
}

UUID::UUID(uint64_t uuid) : _uuid(uuid)
{
}

std::string UUID::ToString() const
{
    std::ostringstream ss;
    ss << std::hex << std::uppercase << _uuid;
    return ss.str();
}

DGEX_END
