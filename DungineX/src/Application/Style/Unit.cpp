#include "DgeX/Application/Style/Unit.h"

#include "DgeX/Core/Log.h"
#include "DgeX/Utils/String.h"

DGEX_BEGIN

Unit Unit::FromString(const char* str)
{
    float value = 0.0f;
    char unitBuffer[16];

    if (sscanf(str, "%f%s", &value, unitBuffer) == 2)
    {
        if (Utils::String::Equals(unitBuffer, "px"))
        {
            return { UnitType::Pixel, value };
        }
        if (Utils::String::Equals(unitBuffer, "%"))
        {
            return { UnitType::Percent, value };
        }
    }
    else if (sscanf(str, "%f", &value) == 1)
    {
        return { UnitType::Pixel, value };
    }

    DGEX_LOG_WARN("Failed to parse unit value: {0}", str);

    return {};
}

float Unit::ToPixel(float parentSize) const
{
    if (IsPercent())
    {
        return parentSize * Value * 0.01f;
    }
    if (IsPixel())
    {
        return Value;
    }

    // Auto, return 0.
    return 0.0f;
}

bool Unit::operator==(const Unit& other) const
{
    return (Type == other.Type) && (Value == other.Value);
}

bool Unit::operator!=(const Unit& other) const
{
    return !(*this == other);
}

Unit& Unit::operator*=(float scalar)
{
    Value *= scalar;
    return *this;
}

Unit& Unit::operator/=(float scalar)
{
    Value /= scalar;
    return *this;
}

Unit operator*(const Unit& unit, float scalar)
{
    Unit result = unit;
    result *= scalar;
    return result;
}

Unit operator*(float scalar, const Unit& unit)
{
    return unit * scalar;
}

Unit operator/(const Unit& unit, float scalar)
{
    Unit result = unit;
    result /= scalar;
    return result;
}

DGEX_END
