#pragma once

DGEX_BEGIN

namespace Utils::Random
{

int RandomInt(int min, int max);
float RandomFloat(float min, float max);
double RandomDouble(double min, double max);
uint64_t RandomUInt64(uint64_t min, uint64_t max);
uint64_t RandomUInt64();

template <typename T> T RandomNumber(T min, T max);

template <> inline int RandomNumber<int>(int min, int max)
{
    return RandomInt(min, max);
}

template <> inline float RandomNumber<float>(float min, float max)
{
    return RandomFloat(min, max);
}

template <> inline double RandomNumber<double>(double min, double max)
{
    return RandomDouble(min, max);
}

template <> inline uint64_t RandomNumber<uint64_t>(uint64_t min, uint64_t max)
{
    return RandomUInt64(min, max);
}

} // namespace Utils::Random

DGEX_END
