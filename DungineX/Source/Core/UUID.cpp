#include <random>
#include <sstream>

#include "DgeX/Core/UUID.h"

DGEX_BEGIN

static std::random_device sRandomDevice;
static std::mt19937_64 sEngine(sRandomDevice());
static std::uniform_int_distribution<uint64_t> sUniformDistribution;

UUID::UUID() : _uuid(sUniformDistribution(sEngine))
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
