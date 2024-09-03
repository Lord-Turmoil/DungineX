#pragma once

#include "DgeX/Physics/Core/Types/Vector.h"
#include "DgeX/Utils/Random.h"

DPHX_BEGIN

namespace Utils
{

inline Vector3 RandomVector(const Vector3& min, const Vector3& max)
{
    using namespace DgeX::Utils::Random;
    return { RandomNumber(min.X, max.X), RandomNumber(min.Y, max.Y), RandomNumber(min.Z, max.Z) };
}

} // namespace Utils

DPHX_END
