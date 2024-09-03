#include <sstream>

#include "DgeX/Core/UUID.h"
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
