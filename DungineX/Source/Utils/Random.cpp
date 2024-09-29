#include "DgeX/Utils/Random.h"

#include <random>

DGEX_BEGIN

namespace Utils::Random
{

static std::random_device sRandomDevice;
static std::mt19937_64 sEngine(sRandomDevice());
static std::uniform_int_distribution<uint64_t> sUint64Distribution;
static std::uniform_int_distribution<uint32_t> sUint32Distribution;

template <typename T> void EnsureMinMax(T& min, T& max)
{
    if (min > max)
    {
        std::swap(min, max);
    }
}

int RandomInt(int min, int max)
{
    EnsureMinMax(min, max);
    return min + static_cast<int>(sUint32Distribution(sEngine) % (max - min + 1));
}

float RandomFloat(float min, float max)
{
    EnsureMinMax(min, max);
    return min + (max - min) * (static_cast<float>(sUint32Distribution(sEngine)) /
                                static_cast<float>(std::numeric_limits<uint32_t>::max()));
}

double RandomDouble(double min, double max)
{
    EnsureMinMax(min, max);
    return min + (max - min) * (static_cast<float>(sUint32Distribution(sEngine)) /
                                static_cast<float>(std::numeric_limits<uint32_t>::max()));
}

uint64_t RandomUInt64(uint64_t min, uint64_t max)
{
    EnsureMinMax(min, max);
    return min + sUint64Distribution(sEngine) % (max - min + 1);
}

uint64_t RandomUInt64()
{
    return sUint64Distribution(sEngine);
}

} // namespace Utils::Random

DGEX_END
